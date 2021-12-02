#include <superfx/SuperFxDataBlockClass.hpp>
#include <superfx/InstructionArguments.hpp>
#include <fma/symbol/ConstantNumber.hpp>
#include <fma/symbol/CalculatedNumber.hpp>
#include <fma/core/Number.hpp>
#include <fma/core/String.hpp>
#include <fma/core/Array.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/types/Undefined.hpp>
#include <fma/types/Object.hpp>
#include <fma/plugin/MemoryPluginAdapter.hpp>
#include <fma/core/DataBlock.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/Project.hpp>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace FMA;
using namespace FMA::types;
using namespace FMA::interpret;
using namespace FMA::symbol;
using namespace FMA::superfx;
using namespace FMA::core;


namespace {
  ReferencePtr CALC_CONST(int64_t value) {
    return ReferencePtr(new ConstantNumber(value));
  }
  ReferencePtr CALC_OR(const ReferencePtr &l, const ReferencePtr &r) {
    return ReferencePtr(new CalculatedNumber(l, CalculatedNumber::OR, r));
  }
  ReferencePtr CALC_SUB(const ReferencePtr &l, const ReferencePtr &r) {
    return ReferencePtr(new CalculatedNumber(l, CalculatedNumber::SUB, r));
  }
  ReferencePtr CALC_ADD(const ReferencePtr &l, const ReferencePtr &r) {
    return ReferencePtr(new CalculatedNumber(l, CalculatedNumber::ADD, r));
  }
  ReferencePtr CALC_AND(const ReferencePtr &l, const ReferencePtr &r) {
    return ReferencePtr(new CalculatedNumber(l, CalculatedNumber::AND, r));
  }
  ReferencePtr CALC_ARG(InstructionArgument *arg) {
    switch (arg->valueType) {
    default:
    case InstructionArgument::VALUE_NONE:
      return ReferencePtr(new ConstantNumber(0));

    case InstructionArgument::VALUE_NUMBER:
      return ReferencePtr(new ConstantNumber(arg->number));

    case InstructionArgument::VALUE_SYMBOL:
      return arg->reference;
    }
  }
}

#define INSTRUCTION_START(name) \
  proto->setMember(name, TypePtr(new InternalFunctionValue(name, [](const ContextPtr &context, const GroupedParameterList &params) -> ResultPtr { \
    FMA::plugin::MemoryBlock *block = DataBlockClass::memoryBlock(context); \
    if (block == NULL) { \
      context->log().error() << "Could not access memory block"; \
      return ResultPtr(new Result()); \
    }

#define INSTRUCTION_START_VARIANTS() \
    InstructionArguments args(context, context->getProject(), params); \
    if (!args.isValid()) {

#define INSTRUCTION(name) \
  INSTRUCTION_START(name) \
  INSTRUCTION_START_VARIANTS()

#define VARIANT(name) \
  } else if (args.name()) {

#define IS1BYTE() \
  context->self()->setMember(".superfx_require_1byte_opcode", TypePtr(new Undefined()));

#define REQ1BYTE() \
  context->self()->setMember(".superfx_require_1byte_opcode", NumberClass::createInstance(context, 1)->get());

#define NO1BYTE() \
  if (!context->self()->getMember(".superfx_require_1byte_opcode")->isUndefined()) { \
    context->log().error() << "Opcodes after BRANCH/LOOP must be 1byte opcodes"; \
    IS1BYTE(); \
  }

#define WRITE(byte) \
  NO1BYTE(); \
  block->writeByte(byte)

#define WRITESYM(value, size) \
  NO1BYTE(); \
  block->write(value, size)

#define WRITEREL(value) \
  WRITERELINLINE(value, -1, 0, 0xFF)

#define WRITERELINLINE(value, offset, orValue, andValue) \
  ReferencePtr _pc = block->reference(); \
  ReferencePtr _relAddr = CALC_ADD(CALC_SUB(CALC_ARG(value), _pc), CALC_CONST(offset)); \
  WRITESYM(CALC_OR(CALC_AND(_relAddr, CALC_CONST(andValue)), CALC_CONST(orValue)), 1)

#define ALT1() WRITE(0x3D)
#define ALT2() WRITE(0x3E)
#define ALT3() WRITE(0x3F)

#define END_INSTRUCTION(name) \
    } else {\
      context->log().error() << "Invalid parameters for " << name << " instruction"; \
    } \
    return ResultPtr(new Result()); \
  }))); \
  //proto->setMember("::" name, proto->getMember(name))


// ----------------------------------------------------------------------------
ClassPtr SuperFxDataBlockClass::create(const ModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("SuperFxDataBlock", "SuperFx::SuperFxDataBlock"));
  klass->extends(ClassObject);

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", SuperFxDataBlockClass::initialize)));

  INSTRUCTION("ADC")
    VARIANT(reg) ALT1(); WRITESYM(CALC_OR(CALC_CONST(0x50), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1);
    VARIANT(immediate) ALT3(); WRITESYM(CALC_OR(CALC_CONST(0x50), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1);
  END_INSTRUCTION("ADC")
  INSTRUCTION("ADD")
    VARIANT(reg) IS1BYTE(); WRITESYM(CALC_OR(CALC_CONST(0x50), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1);
    VARIANT(immediate) ALT2(); WRITESYM(CALC_OR(CALC_CONST(0x50), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1);
  END_INSTRUCTION("ADD")
  INSTRUCTION("ALT1")
    VARIANT(implicit) IS1BYTE(); ALT1();
  END_INSTRUCTION("ALT1")
  INSTRUCTION("ALT2")
    VARIANT(implicit) IS1BYTE(); ALT2();
  END_INSTRUCTION("ALT2")
  INSTRUCTION("ALT3")
    VARIANT(implicit) IS1BYTE(); ALT3();
  END_INSTRUCTION("ALT3")
  INSTRUCTION("AND")
    VARIANT(reg) IS1BYTE(); WRITESYM(CALC_OR(CALC_CONST(0x70), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1);
    VARIANT(immediate) ALT2(); WRITESYM(CALC_OR(CALC_CONST(0x70), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1);
  END_INSTRUCTION("AND")
  INSTRUCTION("ASR")
    VARIANT(implicit) IS1BYTE(); WRITE(0x96);
  END_INSTRUCTION("ASR")
  INSTRUCTION("BCC")
    VARIANT(address) WRITE(0x0C); WRITEREL(args.getLeft()); REQ1BYTE();
  END_INSTRUCTION("BCC")
  INSTRUCTION("BCS")
    VARIANT(address) WRITE(0x0D); WRITEREL(args.getLeft()); REQ1BYTE();
  END_INSTRUCTION("BCS")
  INSTRUCTION("BEQ")
    VARIANT(address) WRITE(0x09); WRITEREL(args.getLeft()); REQ1BYTE();
  END_INSTRUCTION("BEQ")
  INSTRUCTION("BGE")
    VARIANT(address) WRITE(0x06); WRITEREL(args.getLeft()); REQ1BYTE();
  END_INSTRUCTION("BGE")
  INSTRUCTION("BIC")
    VARIANT(reg) ALT1(); WRITESYM(CALC_OR(CALC_CONST(0x70), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1);
    VARIANT(immediate) ALT3(); WRITESYM(CALC_OR(CALC_CONST(0x70), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1);
  END_INSTRUCTION("BIC")
  INSTRUCTION("BLT")
    VARIANT(address) WRITE(0x07); WRITEREL(args.getLeft()); REQ1BYTE();
  END_INSTRUCTION("BLT")
  INSTRUCTION("BMI")
    VARIANT(address) WRITE(0x0B); WRITEREL(args.getLeft()); REQ1BYTE();
  END_INSTRUCTION("BMI")
  INSTRUCTION("BNE")
    VARIANT(address) WRITE(0x08); WRITEREL(args.getLeft()); REQ1BYTE();
  END_INSTRUCTION("BNE")
  INSTRUCTION("BPL")
    VARIANT(address) WRITE(0x0A); WRITEREL(args.getLeft()); REQ1BYTE();
  END_INSTRUCTION("BPL")
  INSTRUCTION("BRA")
    VARIANT(address) WRITE(0x05); WRITEREL(args.getLeft()); REQ1BYTE();
  END_INSTRUCTION("BRA")
  INSTRUCTION("BVC")
    VARIANT(address) WRITE(0x0E); WRITEREL(args.getLeft()); REQ1BYTE();
  END_INSTRUCTION("BVC")
  INSTRUCTION("BVS")
    VARIANT(address) WRITE(0x0F); WRITEREL(args.getLeft()); REQ1BYTE();
  END_INSTRUCTION("BVS")
  INSTRUCTION("CACHE")
    VARIANT(implicit) IS1BYTE(); WRITE(0x02);
  END_INSTRUCTION("CACHE")
  INSTRUCTION("CMODE")
    VARIANT(implicit) ALT1(); WRITE(0x4E);
  END_INSTRUCTION("CMODE")
  INSTRUCTION("CMP")
    VARIANT(reg) ALT3(); WRITESYM(CALC_OR(CALC_CONST(0x60), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1);
  END_INSTRUCTION("CMP")
  INSTRUCTION("COLOR")
    VARIANT(implicit) IS1BYTE(); WRITE(0x4E);
  END_INSTRUCTION("COLOR")
  INSTRUCTION("DEC")
    VARIANT(reg) ALT1(); WRITESYM(CALC_OR(CALC_CONST(0xE0), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1);
  END_INSTRUCTION("DEC")
  INSTRUCTION("DIV2")
    VARIANT(implicit) ALT1(); WRITE(0x96);
  END_INSTRUCTION("DIV2")
  INSTRUCTION("FMULT")
    VARIANT(implicit) IS1BYTE(); WRITE(0x9F);
  END_INSTRUCTION("FMULT")
  INSTRUCTION("FROM")
    VARIANT(reg) IS1BYTE(); WRITESYM(CALC_OR(CALC_CONST(0xB0), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1);
  END_INSTRUCTION("FROM")
  INSTRUCTION("GETB")
    VARIANT(implicit) IS1BYTE(); WRITE(0xEF);
  END_INSTRUCTION("GETB")
  INSTRUCTION("GETBH")
    VARIANT(implicit) ALT1(); WRITE(0xEF);
  END_INSTRUCTION("GETBH")
  INSTRUCTION("GETBL")
    VARIANT(implicit) ALT2(); WRITE(0xEF);
  END_INSTRUCTION("GETBL")
  INSTRUCTION("GETBS")
    VARIANT(implicit) ALT3(); WRITE(0xEF);
  END_INSTRUCTION("GETBS")
  INSTRUCTION("GETC")
    VARIANT(implicit) IS1BYTE(); WRITE(0xDF);
  END_INSTRUCTION("GETC")
  INSTRUCTION("HIB")
    VARIANT(implicit) IS1BYTE(); WRITE(0xC0);
  END_INSTRUCTION("HIB")
  INSTRUCTION("IBT")
    VARIANT(regImmediate) WRITESYM(CALC_OR(CALC_CONST(0xA0), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1); WRITESYM(CALC_ARG(args.getRight()), 1);
  END_INSTRUCTION("IBT")
  INSTRUCTION("INC")
    VARIANT(reg) IS1BYTE(); WRITESYM(CALC_OR(CALC_CONST(0xD0), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1);
  END_INSTRUCTION("INC")
  INSTRUCTION("IWT")
    VARIANT(regImmediate) WRITESYM(CALC_OR(CALC_CONST(0xF0), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1); WRITESYM(CALC_ARG(args.getRight()), 2);
  END_INSTRUCTION("IWT")
  INSTRUCTION("JMP")
    VARIANT(reg) IS1BYTE(); WRITESYM(CALC_OR(CALC_CONST(0x90), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1);
  END_INSTRUCTION("JMP")
  INSTRUCTION("LDB")
    VARIANT(reg) ALT1(); WRITESYM(CALC_OR(CALC_CONST(0x40), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1);
  END_INSTRUCTION("LDB")
  INSTRUCTION("LDW")
    VARIANT(reg) IS1BYTE(); WRITESYM(CALC_OR(CALC_CONST(0x40), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1);
  END_INSTRUCTION("LDW")
  INSTRUCTION("LEA")
    VARIANT(regAddress) WRITESYM(CALC_OR(CALC_CONST(0xF0), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1); WRITESYM(CALC_ARG(args.getRight()), 2);
  END_INSTRUCTION("LEA")
  INSTRUCTION("LINK")
    VARIANT(address) WRITERELINLINE(args.getLeft(), -1, 0x90, 0x0F);
  END_INSTRUCTION("LINK")
  INSTRUCTION("LJMP")
    VARIANT(reg) ALT1(); WRITESYM(CALC_OR(CALC_CONST(0x90), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1);
  END_INSTRUCTION("LJMP")
  INSTRUCTION("LM")
    VARIANT(regAddress) ALT1(); WRITESYM(CALC_OR(CALC_CONST(0xF0), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1); WRITESYM(CALC_ARG(args.getRight()), 2);
  END_INSTRUCTION("LM")
  INSTRUCTION("LMS")
    VARIANT(regAddress) ALT1(); WRITESYM(CALC_OR(CALC_CONST(0xA0), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1); WRITESYM(CALC_ARG(args.getRight()), 2);
  END_INSTRUCTION("LMS")
  INSTRUCTION("LMULT")
    VARIANT(implicit) ALT1(); WRITE(0x9F);
  END_INSTRUCTION("LMULT")
  INSTRUCTION("LOOP")
    VARIANT(implicit) IS1BYTE(); WRITE(0x3C); REQ1BYTE();
  END_INSTRUCTION("LOOP")
  INSTRUCTION("LSR")
    VARIANT(implicit) IS1BYTE(); WRITE(0x03);
  END_INSTRUCTION("LSR")
  INSTRUCTION("MERGE")
    VARIANT(implicit) IS1BYTE(); WRITE(0x70);
  END_INSTRUCTION("MERGE")
  INSTRUCTION("MOVE")
    VARIANT(regReg)
      WRITESYM(CALC_OR(CALC_CONST(0x20), CALC_AND(CALC_ARG(args.getRight()), CALC_CONST(0x0F))), 1);
      WRITESYM(CALC_OR(CALC_CONST(0x10), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1);
  END_INSTRUCTION("MOVE")
  INSTRUCTION("MOVES")
    VARIANT(regReg)
      WRITESYM(CALC_OR(CALC_CONST(0x20), CALC_AND(CALC_ARG(args.getRight()), CALC_CONST(0x0F))), 1);
      WRITESYM(CALC_OR(CALC_CONST(0xB0), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1);
  END_INSTRUCTION("MOVES")
  INSTRUCTION("MULT")
    VARIANT(reg) IS1BYTE(); WRITESYM(CALC_OR(CALC_CONST(0x80), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1);
    VARIANT(immediate) ALT2(); WRITESYM(CALC_OR(CALC_CONST(0x80), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1);
  END_INSTRUCTION("MULT")
  INSTRUCTION("NOP")
    VARIANT(implicit) IS1BYTE(); WRITE(0x01);
  END_INSTRUCTION("NOP")
  INSTRUCTION("NOT")
    VARIANT(implicit) IS1BYTE(); WRITE(0x4F);
  END_INSTRUCTION("NOT")
  INSTRUCTION("OR")
    VARIANT(reg) IS1BYTE(); WRITESYM(CALC_OR(CALC_CONST(0xC0), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1);
    VARIANT(immediate) ALT2(); WRITESYM(CALC_OR(CALC_CONST(0xC0), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1);
  END_INSTRUCTION("OR")
  INSTRUCTION("PLOT")
    VARIANT(implicit) IS1BYTE(); WRITE(0x4C);
  END_INSTRUCTION("PLOT")
  INSTRUCTION("RAMB")
    VARIANT(implicit) ALT2(); WRITE(0xDF);
  END_INSTRUCTION("RAMB")
  INSTRUCTION("ROL")
    VARIANT(implicit) IS1BYTE(); WRITE(0x04);
  END_INSTRUCTION("ROL")
  INSTRUCTION("ROMB")
    VARIANT(implicit) ALT3(); WRITE(0xDF);
  END_INSTRUCTION("ROMB")
  INSTRUCTION("ROR")
    VARIANT(implicit) IS1BYTE(); WRITE(0x97);
  END_INSTRUCTION("ROR")
  INSTRUCTION("RPIX")
    VARIANT(implicit) ALT1(); WRITE(0x4C);
  END_INSTRUCTION("RPIX")
  INSTRUCTION("SBC")
    VARIANT(reg) ALT1(); WRITESYM(CALC_OR(CALC_CONST(0x60), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1);
  END_INSTRUCTION("SBC")
  INSTRUCTION("SBK")
    VARIANT(implicit) IS1BYTE(); WRITE(0x90);
  END_INSTRUCTION("SBK")
  INSTRUCTION("SEX")
    VARIANT(implicit) IS1BYTE(); WRITE(0x95);
  END_INSTRUCTION("SEX")
  INSTRUCTION("SM")
    VARIANT(regAddress) ALT2(); WRITESYM(CALC_OR(CALC_CONST(0xF0), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1); WRITESYM(CALC_ARG(args.getRight()), 2);
  END_INSTRUCTION("SM")
  INSTRUCTION("SMS")
    VARIANT(regAddress) ALT2(); WRITESYM(CALC_OR(CALC_CONST(0xA0), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1); WRITESYM(CALC_ARG(args.getRight()), 2);
  END_INSTRUCTION("SMS")
  INSTRUCTION("STB")
    VARIANT(reg) ALT1(); WRITESYM(CALC_OR(CALC_CONST(0x30), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1);
  END_INSTRUCTION("STB")
  INSTRUCTION("STOP")
    VARIANT(implicit) IS1BYTE(); WRITE(0x00);
  END_INSTRUCTION("STOP")
  INSTRUCTION("STW")
    VARIANT(reg) IS1BYTE(); WRITESYM(CALC_OR(CALC_CONST(0x30), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1);
  END_INSTRUCTION("STW")
  INSTRUCTION("SUB")
    VARIANT(reg) IS1BYTE(); WRITESYM(CALC_OR(CALC_CONST(0x60), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1);
    VARIANT(immediate) ALT2(); WRITESYM(CALC_OR(CALC_CONST(0x60), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1);
  END_INSTRUCTION("SUB")
  INSTRUCTION("TO")
    VARIANT(reg) IS1BYTE(); WRITESYM(CALC_OR(CALC_CONST(0x10), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1);
  END_INSTRUCTION("TO")
  INSTRUCTION("UMULT")
    VARIANT(reg) ALT1(); WRITESYM(CALC_OR(CALC_CONST(0x80), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1);
    VARIANT(immediate) ALT3(); WRITESYM(CALC_OR(CALC_CONST(0x80), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1);
  END_INSTRUCTION("UMULT")
  INSTRUCTION("WITH")
    VARIANT(reg) IS1BYTE(); WRITESYM(CALC_OR(CALC_CONST(0x20), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1);
  END_INSTRUCTION("WITH")
  INSTRUCTION("XOR")
    VARIANT(reg) ALT1(); WRITESYM(CALC_OR(CALC_CONST(0xC0), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1);
    VARIANT(immediate) ALT3(); WRITESYM(CALC_OR(CALC_CONST(0xC0), CALC_AND(CALC_ARG(args.getLeft()), CALC_CONST(0x0F))), 1);
  END_INSTRUCTION("XOR")

  root->setMember("SuperFxDataBlock", klass);
  return klass;
}

// ----------------------------------------------------------------------------
TypePtr SuperFxDataBlockClass::createInstance(const interpret::ContextPtr &context) {
  ClassPtr DataBlock = context->getRootLevelContext()->getMember("SuperFx")->get()->getMember("SuperFxDataBlock")->asClass();
  if (!DataBlock) {
    TypePtr result(new Undefined());
    return result;
  }

  GroupedParameterList emptyParams;
  return DataBlock->createInstance(context, emptyParams);
}

// ----------------------------------------------------------------------------
ResultPtr SuperFxDataBlockClass::initialize(const ContextPtr &, const GroupedParameterList &) {
  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
ResultPtr SuperFxDataBlockClass::__invalid(const ContextPtr &context, const GroupedParameterList &) {
  context->log().error() << "Opcode not supported in SuperFX context";
  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
ResultPtr SuperFxDataBlockClass::__test(const ContextPtr &context, const GroupedParameterList &) {
  FMA::plugin::MemoryBlock *block = DataBlockClass::memoryBlock(context);
  block->writeByte(0x01);
  return ResultPtr(new Result());
}

