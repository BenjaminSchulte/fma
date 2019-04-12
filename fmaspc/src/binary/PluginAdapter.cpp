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
using namespace FMASPC::binary;

#define OPERAND_TYPE(operandName, func) \
  void SpcPluginAdapter::operandName(uint8_t opcode, const std::string &name) { \
    map[name] = ByteCodeGenerator(opcode, [](const ByteCodeGenerator &generator, FMA::assem::BinaryCodeGeneratorScope *scope, FMA::assem::Instruction *instruct) -> bool  func); \
  }

// ----------------------------------------------------------------------------
SpcPluginAdapter::SpcPluginAdapter(Project *project)
  : BinaryGeneratorPluginAdapter(project)
{
  label     (0x00, ".LABEL");
  rep       (0x00, "ANDNOT(P,#)");
  sep       (0x00, "OR(P,#)");


  implicit  (0x00, "NOP");
  branchBit (0x03, "JBS([dp(#)],[la(#)],#)");
  directPage(0x04, "OR(A,[dp(#)])");
  absolute  (0x05, "OR([la(#)],A)");
  implicit  (0x06, "OR(A,[la(X)])");
  directPage(0x07, "OR(A,[la([dp((#+X))])])");
  immediate (0x08, "OR(A,#)");
  dpCopy    (0x09, "OR([dp(#)],[dp(#)])");
  directPage(0x0B, "ASL([dp(#)],#)");
  absolute  (0x0C, "ASL(la(#),#)");
  implicit  (0x0F, "BRK");
  pcRelative(0x10, "JNS(pcrel(#))");
  clrSetBit (0x12, "ANDNOT([dp(#)],#)");
  branchBit (0x13, "JBC([dp(#)],[la(#)],#)");
  absolute  (0x15, "OR(A,[la((#+X))])");
  absolute  (0x16, "OR(A,[la((#+Y))])");
  dpImm     (0x18, "OR([dp(#)],#)");
  directPage(0x1A, "DECW([dp(#)])");
  immediate (0x1C, "ASL(A,#)");
  implicit  (0x1D, "DEC(X)");
  directPage(0x24, "AND(A,[dp(#)])");
  absolute  (0x25, "AND([la(#)],A)");
  implicit  (0x26, "AND(A,[la(X)])");
  directPage(0x27, "AND(A,[la([dp((#+X))])])");
  immediate (0x28, "AND(A,#)");
  dpCopy    (0x29, "AND([dp(#)],[dp(#)])");
  directPage(0x2B, "ROL([dp(#)],#)");
  absolute  (0x2C, "ROL(la(#),#)");
  immediate (0x2D, "PUSH(A)");
  dpPcRel   (0x2E, "CMPJNE(A,[dp(#)],[la(#)])");
  pcRelative(0x2F, "JMP(pcrel(#))");
  pcRelative(0x30, "JS(pcrel(#))");
  absolute  (0x35, "AND(A,[la((#+X))])");
  absolute  (0x36, "AND(A,[la((#+Y))])");
  dpImm     (0x38, "AND([dp(#)],#)");
  directPage(0x3A, "INCW([dp(#)])");
  immediate (0x3C, "ROL(A,#)");
  implicit  (0x3D, "INC(X)");
  directPage(0x3E, "CMP(X,[dp(#)])");
  absolute  (0x3F, "CALL(la(#))");
  directPage(0x44, "EOR(A,[dp(#)])");
  absolute  (0x45, "EOR([la(#)],A)");
  implicit  (0x46, "EOR(A,[la(X)])");
  directPage(0x47, "EOR(A,[la([dp((#+X))])])");
  immediate (0x48, "EOR(A,#)");
  dpCopy    (0x49, "EOR([dp(#)],[dp(#)])");
  directPage(0x4B, "LSR([dp(#)],#)");
  absolute  (0x4C, "LSR(la(#),#)");
  immediate (0x4D, "PUSH(A)");
  absolute  (0x55, "EOR(A,[la((#+X))])");
  absolute  (0x56, "EOR(A,[la((#+Y))])");
  dpImm     (0x58, "EOR([dp(#)],#)");
  directPage(0x5A, "CMPW(YA,[dp(#)])");
  immediate (0x5C, "LSR(A,#)");
  implicit  (0x5D, "MOV(X,A)");
  absolute  (0x5F, "JMP(#)");
  directPage(0x64, "CMP(A,[dp(#)])");
  directPage(0x67, "OR(A,[la([dp((#+X))])])");
  immediate (0x68, "CMP(A,#)");
  dpCopy    (0x69, "CMP([dp(#)],[dp(#)])");
  directPage(0x6B, "ROR([dp(#)],#)");
  absolute  (0x6C, "ROR(la(#),#)");
  immediate (0x6D, "PUSH(Y)");
  dpPcRelNoA(0x6E, "DECJNZ([dp(#)],[la(#)])");
  implicit  (0x6F, "RTS");
  directPage(0x7A, "ADDW(YA,[dp(#)])");
  immediate (0x7C, "ROR(A,#)");
  implicit  (0x7D, "MOV(A,X)");
  directPage(0x7E, "CMP(Y,[dp(#)])");
  directPage(0x84, "ADC(A,[dp(#)])");
  absolute  (0x85, "ADC([la(#)],A)");
  implicit  (0x86, "ADC(A,[la(X)])");
  directPage(0x87, "ADC(A,[la([dp((#+X))])])");
  immediate (0x88, "ADC(A,#)");
  dpCopy    (0x89, "ADC([dp(#)],[dp(#)])");
  directPage(0x8B, "DEC([dp(#)])");
  absolute  (0x8C, "DEC([la(#)])");
  immediate (0x8D, "MOV(Y,#)");
  dpImm     (0x8F, "MOV([dp(#)],#)");
  pcRelative(0x90, "JNC(pcrel(#))");
  absolute  (0x95, "ADC(A,[la((#+X))])");
  absolute  (0x96, "ADC(A,[la((#+Y))])");
  dpImm     (0x98, "ADC([dp(#)],#)");
  directPage(0x9A, "SUBW(YA,[dp(#)])");
  implicit  (0x9C, "DEC(A)");
  implicit  (0x9D, "MOV(X,S)");
  implicit  (0x9E, "DIV(YA,X)");
  implicit  (0x9F, "XCHG((A&#),(A&#))");
  directPage(0xA4, "SBC(A,[dp(#)])");
  absolute  (0xA5, "SBC([la(#)],A)");
  implicit  (0xA6, "SBC(A,[la(X)])");
  directPage(0xA7, "SBC(A,[la([dp((#+X))])])");
  immediate (0xA8, "SBC(A,#)");
  dpCopy    (0xA9, "SBC([dp(#)],[dp(#)])");
  mov1CM    (0xAA, "MOV(C,([la(#)]&#))");
  directPage(0xAB, "INC([dp(#)])");
  absolute  (0xAC, "INC([la(#)])");
  immediate (0xAD, "CMP(Y,#)");
  immediate (0xAE, "POP(A)");
  implicit  (0xAF, "MOVI([la(X)],A)");
  pcRelative(0xB0, "JC(pcrel(#))");
  absolute  (0xB5, "SBC(A,[la((#+X))])");
  absolute  (0xB6, "SBC(A,[la((#+Y))])");
  dpImm     (0xB8, "SBC([dp(#)],#)");
  directPage(0xBA, "MOVW(YA,[dp(#)])");
  implicit  (0xBC, "INC(A)");
  implicit  (0xBD, "MOV(S,X)");
  directPage(0xC4, "MOV([dp(#)],A)");
  absolute  (0xC5, "MOV([la(#)],A)");
  implicit  (0xC6, "MOV([la(X)],A)");
  directPage(0xC7, "MOV([la([dp((#+X))])],A)");
  immediate (0xC8, "CMP(X,#)");
  absolute  (0xC9, "MOV([la(#)],X)");
  mov1MC    (0xCA, "MOV(([la(#)]&#),C)");
  directPage(0xCB, "MOV([dp(#)],Y)");
  absolute  (0xCC, "MOV([la(#)],Y)");
  immediate (0xCD, "MOV(X,#)");
  immediate (0xCE, "POP(X)");
  implicit  (0xCF, "MUL(Y,A)");
  pcRelative(0xD0, "JNZ(pcrel(#))");
  absolute  (0xD5, "MOV([la((#+X))],A)");
  absolute  (0xD6, "MOV([la((#+Y))],A)");
  immediate (0xD7, "MOV([la(([dp(#)]+Y))],A)");
  directPage(0xD8, "MOV([dp(#)],X)");
  directPage(0xDA, "MOVW([dp(#)],YA)");
  implicit  (0xDC, "DEC(Y)");
  implicit  (0xDD, "MOV(A,Y)");
  directPage(0xE4, "MOV(A,[dp(#)])");
  directPage(0xE7, "MOV(A,[la([dp((#+X))])])");
  immediate (0xE8, "MOV(A,#)");
  mov1MC    (0xEA, "NOT(([la(#)]&#),#)");
  directPage(0xEB, "MOV(Y,[dp(#)])");
  immediate (0xEE, "POP(Y)");
  pcRelative(0xF0, "JZ(pcrel(#))");
  absolute  (0xF5, "MOV(A,[la((#+X))])");
  absolute  (0xF6, "MOV(A,[la((#+Y))])");
  immediate (0xF7, "MOV(A,[la(([dp(#)]+Y))])");
  directPage(0xF8, "MOV(X,[dp(#)])");
  dpCopy    (0xFA, "MOV([dp(#)],[dp(#)])");
  implicit  (0xFC, "INC(Y)");
  implicit  (0xFD, "MOV(Y,A)");
  pcRelative(0xFE, "DECJNZ(Y,[la(#)])");
}

// ----------------------------------------------------------------------------
OPERAND_TYPE(label, {
  (void)generator;

  const auto *instr = dynamic_cast<const LabelInstruction*>(instruct);
  scope->getLinkerBlock()->symbol(instr->getSymbol());

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
OPERAND_TYPE(dpPcRel, {
  scope->getLinkerBlock()->write(&generator.opcode, 1);
 
  auto PC = scope->getLinkerBlock()->privateSymbol(scope->getProject()->getMemoryAdapter()->getSymbolMap());

  ReferencePtr label;
  auto labelOperand = instruct->getOperand(2);
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

  scope->getLinkerBlock()->write(instruct->getOperand(1), 1);
  scope->getLinkerBlock()->write(ref, 1);
  return true;
})

// ----------------------------------------------------------------------------
OPERAND_TYPE(dpPcRelNoA, {
  scope->getLinkerBlock()->write(&generator.opcode, 1);
 
  auto PC = scope->getLinkerBlock()->privateSymbol(scope->getProject()->getMemoryAdapter()->getSymbolMap());

  ReferencePtr label;
  auto labelOperand = instruct->getOperand(1);
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

  scope->getLinkerBlock()->write(instruct->getOperand(0), 1);
  scope->getLinkerBlock()->write(ref, 1);
  return true;
})
// ----------------------------------------------------------------------------
OPERAND_TYPE(immediate, {
  scope->getLinkerBlock()->write(&generator.opcode, 1);
  scope->getLinkerBlock()->write(instruct->getNumericOperand(), 1);
  return true;
})

// ----------------------------------------------------------------------------
OPERAND_TYPE(directPage, {
  scope->getLinkerBlock()->write(&generator.opcode, 1);
  scope->getLinkerBlock()->write(instruct->getNumericOperand(), 1);
  return true;
})

// ----------------------------------------------------------------------------
OPERAND_TYPE(absolute, {
  scope->getLinkerBlock()->write(&generator.opcode, 1);
  scope->getLinkerBlock()->write(instruct->getNumericOperand(), 2);
  return true;
})

// ----------------------------------------------------------------------------
OPERAND_TYPE(dpCopy, {
  scope->getLinkerBlock()->write(&generator.opcode, 1);
  scope->getLinkerBlock()->write(instruct->getOperand(0), 1);
  scope->getLinkerBlock()->write(instruct->getOperand(1), 1);
  return true;
})

// ----------------------------------------------------------------------------
OPERAND_TYPE(mov1CM, {
  std::cout << "TODO MOV1" << std::endl;
  scope->getLinkerBlock()->write(&generator.opcode, 1);
  scope->getLinkerBlock()->write(instruct->getOperand(0), 1);
  //scope->getLinkerBlock()->write(instruct->getOperand(2), 1);
  return true;
})

// ----------------------------------------------------------------------------
OPERAND_TYPE(mov1MC, {
  std::cout << "TODO MOV1" << std::endl;
  scope->getLinkerBlock()->write(&generator.opcode, 1);
  scope->getLinkerBlock()->write(instruct->getOperand(0), 1);
  //scope->getLinkerBlock()->write(instruct->getOperand(1), 1);
  return true;
})

// ----------------------------------------------------------------------------
OPERAND_TYPE(dpImm, {
  scope->getLinkerBlock()->write(&generator.opcode, 1);
  scope->getLinkerBlock()->write(instruct->getOperand(0), 1);
  scope->getLinkerBlock()->write(instruct->getOperand(1), 1);
  return true;
})

// ----------------------------------------------------------------------------
OPERAND_TYPE(branchBit, {
  uint8_t opcode = generator.opcode + instruct->getOperand(2)->asConstant() * 0x20;

  scope->getLinkerBlock()->write(&opcode, 1);
  scope->getLinkerBlock()->write(instruct->getOperand(0), 1);
  scope->getLinkerBlock()->write(instruct->getOperand(1), 1);
  return true;
})

// ----------------------------------------------------------------------------
OPERAND_TYPE(clrSetBit, {
  uint8_t opcode = generator.opcode + instruct->getOperand(1)->asConstant() * 0x20;

  scope->getLinkerBlock()->write(&opcode, 1);
  scope->getLinkerBlock()->write(instruct->getOperand(0), 1);
  return true;
})

// ----------------------------------------------------------------------------
OPERAND_TYPE(implicit, {
  (void)instruct;

  scope->getLinkerBlock()->write(&generator.opcode, 1);
  return true;
})

// ----------------------------------------------------------------------------
OPERAND_TYPE(sep, {
  Operand *op = instruct->getNumericOperand();
  if (op->isConstant()) {
    switch (op->asConstant()) {
      case 0x01: scope->getLinkerBlock()->writeNumber(0x80, 1); return true; // SETC
      case 0x20: scope->getLinkerBlock()->writeNumber(0x40, 1); return true; // SETP
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
      case 0x01: scope->getLinkerBlock()->writeNumber(0x60, 1); return true; // CLRC
      case 0x20: scope->getLinkerBlock()->writeNumber(0x20, 1); return true; // CLRP
      case 0x48: scope->getLinkerBlock()->writeNumber(0xE0, 1); return true; // CLRV
    }
  }

  scope->getLinkerBlock()->write(&generator.opcode, 1);
  scope->getLinkerBlock()->write(op, 1);

  return true;
})

// ----------------------------------------------------------------------------
bool SpcPluginAdapter::supports(const std::string &name) const {
  return map.find(name) != map.end();
}

// ----------------------------------------------------------------------------
bool SpcPluginAdapter::generate(BinaryCodeGeneratorScope *scope, Instruction *instruct, const std::string &name) {
  auto it = map.find(name);
  if (it == map.end()) {
    return false;
  }

  return it->second.execute(scope, instruct);
}

// ----------------------------------------------------------------------------
