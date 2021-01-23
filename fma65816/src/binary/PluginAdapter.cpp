#include <fma/assem/BinaryCodeGeneratorScope.hpp>
#include <fma/assem/Instruction.hpp>
#include <fma/assem/LabelInstruction.hpp>
#include <fma/assem/Operand.hpp>
#include <fma/symbol/SignedAssertRangeReference.hpp>
#include <fma/symbol/CalculatedNumber.hpp>
#include <fma/symbol/ConstantNumber.hpp>
#include <fma/linker/LinkerBlock.hpp>
#include <fma/Project.hpp>

#include <binary/PluginAdapter.hpp>
#include <iostream>

using namespace FMA;
using namespace FMA::assem;
using namespace FMA::plugin;
using namespace FMA::symbol;
using namespace FMA65816::binary;

#define OPERAND_TYPE(operandName, func) \
  void PluginAdapter::operandName(uint8_t opcode, const std::string &name) { \
    map[name] = ByteCodeGenerator(opcode, [](const ByteCodeGenerator &generator, FMA::assem::BinaryCodeGeneratorScope *scope, FMA::assem::Instruction *instruct) -> bool  func); \
  }

// ----------------------------------------------------------------------------
PluginAdapter::PluginAdapter(Project *project)
  : BinaryGeneratorPluginAdapter(project)
{
  label     (0x00, ".LABEL");

  brk       (0x00, "BRK");
  directPage(0x01, "OR(A,[la([dp((#+X))])])");
  // COP
  srRelative(0x03, "OR(A,[sp((#+S))])");
  directPage(0x04, "TSB([dp(#)],A)");
  directPage(0x05, "OR(A,[dp(#)])");
  directPage(0x06, "ASL([dp(#)],#)");
  directPage(0x07, "OR(A,[[dp(#)]])");
  implicit  (0x08, "PUSH(P)");
  immediate (0x09, "OR(A,#)");
  implicit  (0x0A, "ASL(A,#)");
  implicit  (0x0B, "PUSH(D)");
  absolute  (0x0C, "TSB(la(#),A)");
  absolute  (0x0D, "OR(A,[la(#)])");
  absolute  (0x0E, "ASL([la(#)],#)");
  absLong   (0x0F, "OR(A,[#])");
  pcRelative(0x10, "JNS(pcrel(#))");
  directPage(0x11, "OR(A,[(la([dp([la([dp(#)])])])+Y)])");
  directPage(0x12, "OR(A,[la([dp(#)])])");
  // ORA
  directPage(0x14, "TRB([dp(#)],A)");
  directPage(0x15, "OR(A,[(la([dp([la([dp(#)])])])+X)])");
  directPage(0x16, "ASL([dp((#+X))],#)");
  directPage(0x17, "OR(A,[([dp([[dp(#)]])]+Y)])");

  absolute  (0x19, "OR(A,[la((#+Y))])");
  implicit  (0x1A, "INC(A)");
  implicit  (0x1B, "MOV(S,C)");
  absolute  (0x1C, "TRB(la(#),A)");
  absolute  (0x1D, "OR(A,[la((#+X))])");
  absolute  (0x1E, "ASL([la((#+X))],#)");
  absLong   (0x1F, "OR(A,[(#+X)])");
  absolute  (0x20, "CALL(la(#))");
  directPage(0x21, "AND(A,[la([dp((#+X))])])");
  absLong   (0x22, "CALL(#)");
  srRelative(0x23, "AND(A,[sp((#+S))])");
  directPage(0x24, "BIT(A,[dp(#)])");
  directPage(0x25, "AND(A,[dp(#)])");
  directPage(0x26, "ROL([dp(#)],#)");
  directPage(0x27, "AND(A,[[dp(#)]])");
  implicit  (0x28, "POP(P)");
  immediate (0x29, "AND(A,#)");
  implicit  (0x2A, "ROL(A,#)");
  implicit  (0x2B, "POP(D)");
  absolute  (0x2C, "BIT(A,[la(#)])");
  absolute  (0x2D, "AND(A,[la(#)])");
  absolute  (0x2E, "ROL([la(#)],#)");
  absLong   (0x2F, "AND(A,[#])");
  pcRelative(0x30, "JS(pcrel(#))");
  directPage(0x31, "AND(A,[(la([dp([la([dp(#)])])])+Y)])");
  directPage(0x32, "AND(A,[la([dp(#)])])");
  // AND

  directPage(0x35, "AND(A,[(la([dp([la([dp(#)])])])+X)])");
  directPage(0x36, "ROL([dp((#+X))],#)");
  directPage(0x37, "AND(A,[([dp([[dp(#)]])]+Y)])");

  absolute  (0x39, "AND(A,[la((#+Y))])");
  implicit  (0x3A, "DEC(A)");
  implicit  (0x3B, "MOV(C,S)");
  absolute  (0x3C, "BIT(A,[la((#+X))])");
  absolute  (0x3D, "AND(A,[la((#+X))])");
  absolute  (0x3E, "ROL([la((#+X))],#)");
  absLong   (0x3F, "AND(A,[(#+X)])");
  implicit  (0x40, "RTI");
  directPage(0x41, "EOR(A,[la([dp((#+X))])])");

  srRelative(0x43, "EOR(A,[sp((#+S))])");
  move      (0x44, "MVP(#,#)");
  directPage(0x45, "EOR(A,[dp(#)])");
  directPage(0x46, "LSR([dp(#)],#)");
  directPage(0x47, "EOR(A,[[dp(#)]])");
  implicit  (0x48, "PUSH(A)");
  immediate (0x49, "EOR(A,#)");
  implicit  (0x4A, "LSR(A,#)");
  implicit  (0x4B, "PUSH(PB)");
  absolute  (0x4C, "JMP(la(#))");
  absolute  (0x4D, "EOR(A,[la(#)])");
  absolute  (0x4E, "LSR([la(#)],#)");
  absLong   (0x4F, "EOR(A,[#])");
  pcRelative(0x50, "JNO(pcrel(#))");
  directPage(0x51, "EOR(A,[(la([dp([la([dp(#)])])])+Y)])");
  directPage(0x52, "EOR(A,[la([dp(#)])])");
  // EOR
  move      (0x54, "MVN(#,#)");
  directPage(0x55, "EOR(A,[(la([dp([la([dp(#)])])])+X)])");
  directPage(0x56, "LSR([dp((#+X))],#)");
  directPage(0x57, "EOR(A,[([dp([[dp(#)]])]+Y)])");

  absolute  (0x59, "EOR(A,[la((#+Y))])");
  implicit  (0x5A, "PUSH(Y)");
  implicit  (0x5B, "MOV(D,C)");
  absLong   (0x5C, "JMP(#)");
  absolute  (0x5D, "EOR(A,[la((#+X))])");
  absolute  (0x5E, "LSR([la((#+X))],#)");
  absLong   (0x5F, "EOR(A,[(#+X)])");
  implicit  (0x60, "RTS");
  directPage(0x61, "ADC(A,[la([dp((#+X))])])");
  // PER
  srRelative(0x63, "ADC(A,[sp((#+S))])");
  stzDp     (0x64, "MOV([dp(#)],#)");
  directPage(0x65, "ADC(A,[dp(#)])");
  directPage(0x66, "ROR([dp(#)],#)");
  directPage(0x67, "ADC(A,[[dp(#)]])");
  implicit  (0x68, "POP(A)");
  immediate (0x69, "ADC(A,#)");
  implicit  (0x6A, "ROR(A,#)");
  implicit  (0x6B, "RTL");

  absolute  (0x6D, "ADC(A,[la(#)])");
  absolute  (0x6E, "ROR([la(#)],#)");
  absLong   (0x6F, "ADC(A,[#])");
  pcRelative(0x70, "JO(pcrel(#))");
  directPage(0x71, "ADC(A,[(la([dp([la([dp(#)])])])+Y)])");
  directPage(0x72, "ADC(A,[la([dp(#)])])");
  // ADC
  stzDp     (0x74, "MOV([dp((#+X))],#)");
  directPage(0x75, "ADC(A,[(la([dp([la([dp(#)])])])+X)])");
  directPage(0x76, "ROR([dp((#+X))],#)");
  directPage(0x77, "ADC(A,[([dp([[dp(#)]])]+Y)])");

  absolute  (0x79, "ADC(A,[la((#+Y))])");
  implicit  (0x7A, "POP(Y)");
  implicit  (0x7B, "MOV(C,D)");
  absolute  (0x7C, "JMP([(la([dp([la([dp(#)])])])+X)])");
  absolute  (0x7D, "ADC(A,[la((#+X))])");
  absolute  (0x7E, "ROR([la((#+X))],#)");
  absLong   (0x7F, "ADC(A,[(#+X)])");
  pcRelative(0x80, "JMP(pcrel(#))");
  directPage(0x81, "MOV([la([dp((#+X))])],A)");

  srRelative(0x83, "MOV([sp((#+S))],A)");
  directPage(0x84, "MOV([dp(#)],Y)");
  directPage(0x85, "MOV([dp(#)],A)");
  directPage(0x86, "MOV([dp(#)],X)");
  directPage(0x87, "MOV([[dp(#)]],A)");
  implicit  (0x88, "DEC(Y)");
  immediate (0x89, "BIT(A,#)");
  implicit  (0x8A, "MOV(A,X)");
  implicit  (0x8B, "PUSH(DB)");
  absolute  (0x8C, "MOV([la(#)],Y)");
  absolute  (0x8D, "MOV([la(#)],A)");
  absolute  (0x8E, "MOV([la(#)],X)");
  absLong   (0x8F, "MOV([#],A)");
  pcRelative(0x90, "JNC(pcrel(#))");
  directPage(0x91, "MOV([(la([dp([la([dp(#)])])])+Y)],A)");
  directPage(0x92, "MOV([la([dp(#)])],A)");
  // MOV

  directPage(0x95, "MOV([dp((#+X))],A)");

  directPage(0x97, "MOV([([dp([[dp(#)]])]+Y)],A)");
  implicit  (0x98, "MOV(A,Y)");
  absolute  (0x99, "MOV([la((#+Y))],A)");
  implicit  (0x9A, "MOV(S,X)");
  implicit  (0x9B, "MOV(Y,X)");
  stzAbs    (0x9C, "MOV([la(#)],#)");
  absolute  (0x9D, "MOV([la((#+X))],A)");
  stzAbs    (0x9E, "MOV([la((#+X))],#)");
  absLong   (0x9F, "MOV([(#+X)],A)");
  immediate (0xA0, "MOV(Y,#)");
  directPage(0xA1, "MOV(A,[la([dp((#+X))])])");
  immediate (0xA2, "MOV(X,#)");
  srRelative(0xA3, "MOV(A,[sp((#+S))])");
  directPage(0xA4, "MOV(Y,[dp(#)])");
  directPage(0xA5, "MOV(A,[dp(#)])");
  directPage(0xA6, "MOV(X,[dp(#)])");
  directPage(0xA7, "MOV(A,[[dp(#)]])");
  implicit  (0xA8, "MOV(Y,A)");
  immediate (0xA9, "MOV(A,#)");
  implicit  (0xAA, "MOV(X,A)");
  implicit  (0xAB, "POP(DB)");
  absolute  (0xAC, "MOV(Y,[la(#)])");
  absolute  (0xAD, "MOV(A,[la(#)])");
  absolute  (0xAE, "MOV(X,[la(#)])");
  absLong   (0xAF, "MOV(A,[#])");
  pcRelative(0xB0, "JC(pcrel(#))");
  directPage(0xB1, "MOV(A,[(la([dp([la([dp(#)])])])+Y)])");
  directPage(0xB2, "MOV(A,[la([dp(#)])])");
  // MOV
  directPage(0xB4, "MOV(Y,[dp((#+X))])");
  directPage(0xB5, "MOV(A,[dp((#+X))])");
  absolute  (0xB6, "MOV(X,[dp((#+Y))])");
  directPage(0xB7, "MOV(A,[([dp([[dp(#)]])]+Y)])");

  absolute  (0xB9, "MOV(A,[la((#+Y))])");
  implicit  (0xBA, "MOV(X,S)");
  implicit  (0xBB, "MOV(X,Y)");
  absolute  (0xBC, "MOV(Y,[la((#+X))])");
  absolute  (0xBD, "MOV(A,[la((#+X))])");
  absolute  (0xBE, "MOV(X,[la((#+Y))])");
  absLong   (0xBF, "MOV(A,[(#+X)])");
  immediate (0xC0, "CMP(Y,#)");
  directPage(0xC1, "CMP(A,[la([dp((#+X))])])");
  rep       (0xC2, "ANDNOT(P,#)");
  srRelative(0xC3, "CMP(A,[sp((#+S))])");
  directPage(0xC4, "CMP(Y,[dp(#)])");
  directPage(0xC5, "CMP(A,[dp(#)])");
  directPage(0xC6, "DEC([dp(#)])");
  directPage(0xC7, "CMP(A,[[dp(#)]])");
  implicit  (0xC8, "INC(Y)");
  immediate (0xC9, "CMP(A,#)");
  implicit  (0xCA, "DEC(X)");
  implicit  (0xCB, "WAI");
  absolute  (0xCC, "CMP(Y,[la(#)])");
  absolute  (0xCD, "CMP(A,[la(#)])");
  absolute  (0xCE, "DEC([la(#)])");
  absLong   (0xCF, "CMP(A,[#])");
  pcRelative(0xD0, "JNZ(pcrel(#))");
  directPage(0xD1, "CMP(A,[(la([dp([la([dp(#)])])])+Y)])");
  directPage(0xD2, "CMP(A,[la([dp(#)])])");
  // CMP (sr,S),Y	
  directPage(0xD4, "PUSH([dp(#)])");
  directPage(0xD5, "CMP(A,[(la([dp([la([dp(#)])])])+X)])");

  directPage(0xD7, "CMP(A,[([dp([[dp(#)]])]+Y)])");

  absolute  (0xD9, "CMP(A,[la((#+Y))])");
  implicit  (0xDA, "PUSH(X)");
  implicit  (0xDB, "STP");

  absolute  (0xDD, "CMP(A,[la((#+X))])");
  absolute  (0xDE, "DEC([la((#+X))])");
  absLong   (0xDF, "CMP(A,[(#+X)])");
  immediate (0xE0, "CMP(X,#)");
  directPage(0xE1, "SBC(A,[la([dp((#+X))])])");
  sep       (0xE2, "OR(P,#)");
  srRelative(0xE3, "SBC(A,[sp((#+S))])");
  directPage(0xE4, "CMP(X,[dp(#)])");
  directPage(0xE5, "SBC(A,[dp(#)])");
  directPage(0xE6, "INC([dp(#)])");
  directPage(0xE7, "SBC(A,[[dp(#)]])");
  implicit  (0xE8, "INC(X)");
  immediate (0xE9, "SBC(A,#)");
  implicit  (0xEA, "NOP");
  implicit  (0xEB, "XCHG(B,A)");
  absolute  (0xEC, "CMP(X,[la(#)])");
  absolute  (0xED, "SBC(A,[la(#)])");
  absolute  (0xEE, "INC([la(#)])");
  absLong   (0xEF, "SBC(A,[#])");
  pcRelative(0xF0, "JZ(pcrel(#))");
  directPage(0xF1, "SBC(A,[(la([dp([la([dp(#)])])])+Y)])");
  directPage(0xF2, "SBC(A,[la([dp(#)])])");
  // SBC
  immediate (0xF4, "PUSH(#)");
  directPage(0xF5, "SBC(A,[(la([dp([la([dp(#)])])])+X)])");

  directPage(0xF7, "SBC(A,[([dp([[dp(#)]])]+Y)])");

  absolute  (0xF9, "SBC(A,[la((#+Y))])");
  implicit  (0xFA, "POP(X)");
  implicit  (0xFB, "XCHG(E,C)");
  absolute  (0xFC, "CALL([(la([dp([la([dp(#)])])])+X)])");
  absolute  (0xFD, "SBC(A,[la((#+X))])");
  absolute  (0xFE, "INC([la((#+X))])");
  absLong   (0xFF, "SBC(A,[(#+X)])");
}

// ----------------------------------------------------------------------------
OPERAND_TYPE(label, {
  (void)generator;

  const auto *instr = dynamic_cast<const LabelInstruction*>(instruct);
  scope->getLinkerBlock()->symbol(instr->getSymbol());

  return true;
})

// ----------------------------------------------------------------------------
OPERAND_TYPE(absolute, {
  scope->getLinkerBlock()->write(&generator.opcode, 1);
  scope->getLinkerBlock()->write(instruct->getNumericOperand(), 2);
  return true;
})

// ----------------------------------------------------------------------------
OPERAND_TYPE(stzAbs, {
  if (!instruct->getOperand(1)->isConstant() || instruct->getOperand(1)->asConstant() != 0) {
    return false;
  }

  scope->getLinkerBlock()->write(&generator.opcode, 1);
  scope->getLinkerBlock()->write(instruct->getOperand(0), 2);
  return true;
})

// ----------------------------------------------------------------------------
OPERAND_TYPE(stzDp, {
  if (!instruct->getOperand(1)->isConstant() || instruct->getOperand(1)->asConstant() != 0) {
    return false;
  }

  scope->getLinkerBlock()->write(&generator.opcode, 1);
  scope->getLinkerBlock()->write(instruct->getOperand(0), 1);
  return true;
})

// ----------------------------------------------------------------------------
OPERAND_TYPE(absLong, {
  scope->getLinkerBlock()->write(&generator.opcode, 1);
  scope->getLinkerBlock()->write(instruct->getNumericOperand(), 3);
  return true;
})

// ----------------------------------------------------------------------------
OPERAND_TYPE(directPage, {
  scope->getLinkerBlock()->write(&generator.opcode, 1);
  scope->getLinkerBlock()->write(instruct->getNumericOperand(), 1);
  return true;
})

// ----------------------------------------------------------------------------
OPERAND_TYPE(pcRelative, {
  scope->getLinkerBlock()->write(&generator.opcode, 1);
 
  auto PC = scope->getLinkerBlock()->privateSymbol(scope->getProject()->getMemoryAdapter()->getSymbolMap());

  ReferencePtr label;
  auto labelOperand = instruct->getNumericOperand();
  if (labelOperand->isSymbolReference()) {
    label = labelOperand->asSymbolReference();
  } else if (labelOperand->isConstant()) {
    label = ReferencePtr(new ConstantNumber(labelOperand->asConstant()));
  } else {
    return false;
  }

  ReferencePtr relative(new CalculatedNumber(
    label,
    CalculatedNumber::SUB,
    ReferencePtr(new CalculatedNumber(PC, CalculatedNumber::ADD, ReferencePtr(new ConstantNumber(1))))
  ));
  
  ReferencePtr ref(new SignedAssertRangeReference(relative, -128, 127));

  scope->getLinkerBlock()->write(ref, 1);
  return true;
})

// ----------------------------------------------------------------------------
OPERAND_TYPE(srRelative, {
  scope->getLinkerBlock()->write(&generator.opcode, 1);
  scope->getLinkerBlock()->write(instruct->getNumericOperand(), 1);
  return true;
})

// ----------------------------------------------------------------------------
OPERAND_TYPE(brk, {
  (void)instruct;
  uint8_t zero = 0xEA; // nop
  scope->getLinkerBlock()->write(&generator.opcode, 1);
  scope->getLinkerBlock()->write(&zero, 1);
  return true;
})

// ----------------------------------------------------------------------------
OPERAND_TYPE(implicit, {
  (void)instruct;
  scope->getLinkerBlock()->write(&generator.opcode, 1);
  return true;
})

// ----------------------------------------------------------------------------
OPERAND_TYPE(immediate, {
  if (!instruct->hasAttribute("immediateSize")) {
    return false;
  }

  uint8_t size = boost::any_cast<uint8_t>(instruct->getAttribute("immediateSize"));

  scope->getLinkerBlock()->write(&generator.opcode, 1);
  switch (size) {
  case 8:
    scope->getLinkerBlock()->write(instruct->getNumericOperand(), 1);
    break;
  case 16:
    scope->getLinkerBlock()->write(instruct->getNumericOperand(), 2);
    break;
  default:
    return false;
  }

  return true;
})

// ----------------------------------------------------------------------------
OPERAND_TYPE(move, {
  scope->getLinkerBlock()->write(&generator.opcode, 1);
  scope->getLinkerBlock()->write(instruct->getOperand(1), 1);
  scope->getLinkerBlock()->write(instruct->getOperand(0), 1);

  return true;
})

// ----------------------------------------------------------------------------
OPERAND_TYPE(sep, {
  Operand *op = instruct->getNumericOperand();
  if (op->isConstant()) {
    switch (op->asConstant()) {
      case 0x01: scope->getLinkerBlock()->writeNumber(0x38, 1); return true; // SEC
      case 0x04: scope->getLinkerBlock()->writeNumber(0x78, 1); return true; // SEI
      case 0x08: scope->getLinkerBlock()->writeNumber(0xF8, 1); return true; // SED
    }
  }

  scope->getLinkerBlock()->write(&generator.opcode, 1);
  scope->getLinkerBlock()->write(op, 1);

  return true;
})

// ----------------------------------------------------------------------------
OPERAND_TYPE(rep, {
  Operand *op = instruct->getNumericOperand();
  if (op->isConstant()) {
    switch (op->asConstant()) {
      case 0x01: scope->getLinkerBlock()->writeNumber(0x18, 1); return true; // CLC
      case 0x04: scope->getLinkerBlock()->writeNumber(0x58, 1); return true; // CLI
      case 0x08: scope->getLinkerBlock()->writeNumber(0xD8, 1); return true; // CLD
      case 0x40: scope->getLinkerBlock()->writeNumber(0xB8, 1); return true; // CLV
    }
  }

  scope->getLinkerBlock()->write(&generator.opcode, 1);
  scope->getLinkerBlock()->write(op, 1);

  return true;
})

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
