#include <fma/assem/BinaryCodeGeneratorScope.hpp>
#include <fma/assem/LabelInstruction.hpp>
#include <fma/instruct/BinaryData.hpp>
#include <fma/instruct/DynamicBinaryData.hpp>
#include <fma/instruct/RawCode.hpp>
#include <fma/assem/SymbolReferenceOperand.hpp>
#include <fma/symbol/SymbolReference.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/Project.hpp>

#include <binary/PluginAdapter.hpp>
#include <binary/CodeSection.hpp>
#include <iostream>

using namespace FMA;
using namespace FMA::assem;
using namespace FMA::instruct;
using namespace FMACPP::binary;

#define OPERAND_TYPE(operandName, func) \
  void PluginAdapter::operandName(const std::string &opcode, const std::string &name) { \
    map[name] = ByteCodeGenerator(opcode, [this](const ByteCodeGenerator &generator, FMA::assem::BinaryCodeGeneratorScope *scope, FMA::assem::Instruction *instruct) -> bool  func); \
  }
#define ASSERT_VALID_CODE_SECTION() \
  if (!mCurrentCodeSection) { \
    project->log().warn() << "Code outside of a code section"; \
    return false; \
  }

// ----------------------------------------------------------------------------
PluginAdapter::PluginAdapter(Project *project)
  : BinaryGeneratorPluginAdapter(project)
{
  label     (".LABEL", ".LABEL");
  data      (".DATA", "$DATA");
  symbol    (".SYMBOL", "$SYM");
  raw       (".RAW", "$RAW");

  codeReturn("  return RTI();", "RTI");
  codeReturn("  return RTS();", "RTS");
  codeReturn("  return $1;", "JMP(pcrel(#))");
  codeReturn("  return $1;", "JMP(#)");
  codeBreak ("  return CALL($1, #);", "CALL(la(#))");

  code      ("  SEP($2);", "OR(P,#)");
  code      ("  REP($2);", "ANDNOT(P,#)");
  code      ("  R_S = R_A;", "MOV(S,C)");

  code      ("  P_M ? LDA8($2) : LDA16($2);", "MOV(A,#)");
  code      ("  P_M ? LDA8(dataRead8(currentBank($2))) : LDA16(dataRead16(currentBank($2)));", "MOV(A,[la(#)])");
  code      ("  P_X ? LDX8($2) : LDX16($2);", "MOV(X,#)");
  code      ("  P_M ? dataWrite8($1, R_A) : dataWrite16($1, R_A);", "MOV([#],A)");
  code      ("  P_M ? dataWrite8(currentBank($1), R_A) : dataWrite16(currentBank($1), R_A);", "MOV([la(#)],A)");
  code      ("  P_M ? dataWrite8(currentBank($1), $2) : dataWrite16(currentBank($1), $2);", "MOV([la(#)],#)");


  ignore    ("", "NOP");
  ignore    ("", "XCHG(E,C)");

}

// ----------------------------------------------------------------------------
PluginAdapter::~PluginAdapter() {
  for (CodeSection *section : mCodeSections) {
    delete section;
  }
}

// ----------------------------------------------------------------------------
bool PluginAdapter::supports(const std::string &name) const {
  return map.find(name) != map.end();
}

// ----------------------------------------------------------------------------
bool PluginAdapter::generate(BinaryCodeGeneratorScope *scope, Instruction *instruct, const std::string &name) {
  auto it = map.find(name);
  if (it == map.end()) {
    return false;
  }

  return it->second.execute(scope, instruct);
}

// ----------------------------------------------------------------------------
bool PluginAdapter::finish() {
  for (CodeSection *section : mCodeSections) {
    if (!section->generate()) {
      return false;
    }
  }

  return true;
}

// ----------------------------------------------------------------------------
OPERAND_TYPE(label, {
  (void)generator;

  CodeSection *previousSection = mCurrentCodeSection;

  const auto *instr = dynamic_cast<const LabelInstruction*>(instruct);
  CodeSection *section = new CodeSection(mCodeSections.size(), instr->getSymbol(), scope->getLinkerBlock());
  mCurrentCodeSection = section;
  mCodeSections.push_back(section);

  if (previousSection && previousSection->isCodeSection()) {
    FMA::symbol::SymbolReferencePtr ref(new FMA::symbol::SymbolReference(section->label()));
    previousSection->writeCode("  return __routine_");
    previousSection->writeCode(ref);
    previousSection->writeCode("();\n");
  }
  return true;
})

// ----------------------------------------------------------------------------
OPERAND_TYPE(data, {
  ASSERT_VALID_CODE_SECTION();

  (void)scope;
  (void)generator;

  BinaryData *data = dynamic_cast<BinaryData*>(instruct);
  mCurrentCodeSection->writeData(data->getData(), data->getSize());
  return true;
})

// ----------------------------------------------------------------------------
OPERAND_TYPE(raw, {
  ASSERT_VALID_CODE_SECTION();

  (void)scope;
  (void)generator;

  RawCode *raw = dynamic_cast<RawCode*>(instruct);
  mCurrentCodeSection->writeCode(raw->getData());
  return true;
})

// ----------------------------------------------------------------------------
OPERAND_TYPE(symbol, {
  ASSERT_VALID_CODE_SECTION();

  (void)scope;
  (void)generator;

  DynamicBinaryData *data = dynamic_cast<DynamicBinaryData*>(instruct);
  mCurrentCodeSection->writeData(data->getData(), data->getSize());
  return true;
})

// ----------------------------------------------------------------------------
OPERAND_TYPE(ignore, {
  (void)this;
  (void)scope;
  (void)instruct;
  (void)generator;
  return true;
})

// ----------------------------------------------------------------------------
bool PluginAdapter::writeCode(CodeSection *section, const std::string &opcode, FMA::assem::Instruction *instruct, bool nl) {
  std::size_t left = 0;

  while (true) {
    std::size_t right = opcode.find_first_of('$', left);
    if (right == std::string::npos) {
      break;
    }

    if (right > left) {
      section->writeCode(opcode.substr(left, right - left));
    }

    int instructionIndex = (int)((char)opcode[right + 1] - '1');
    section->writeCode(instruct->getOperand(instructionIndex));
    left = right + 2;
  }

  section->writeCode(opcode.substr(left));
  if (nl) {section->writeCode("\n");}
  return true;
}

// ----------------------------------------------------------------------------
OPERAND_TYPE(code, {
  ASSERT_VALID_CODE_SECTION();
  (void)scope;

  return writeCode(mCurrentCodeSection, generator.opcode, instruct, true);
})

// ----------------------------------------------------------------------------
OPERAND_TYPE(codeReturn, {
  ASSERT_VALID_CODE_SECTION();
  (void)scope;

  if (!writeCode(mCurrentCodeSection, generator.opcode, instruct, true)) {
    return false;
  }

  mCurrentCodeSection = nullptr;
  return true;
})

// ----------------------------------------------------------------------------
OPERAND_TYPE(codeBreak, {
  ASSERT_VALID_CODE_SECTION();
  (void)scope;

  CodeSection *previousSection = mCurrentCodeSection;
  previousSection->markAsCode();

  // Creates a new label
  CodeSection *section = new CodeSection(mCodeSections.size(), previousSection->label() + "__" + std::to_string(mNextLabelId++), scope->getLinkerBlock());
  section->markAsCode();
  mCurrentCodeSection = section;
  mCodeSections.push_back(section);

  int index = generator.opcode.find_first_of('#');
  if (!writeCode(previousSection, generator.opcode.substr(0, index), instruct, false)) {
    return false;
  }
  
  FMA::symbol::SymbolReferencePtr ref(new FMA::symbol::SymbolReference(section->label()));
  previousSection->writeCode(ref);

  return writeCode(previousSection, generator.opcode.substr(index + 1), instruct, true);
})
