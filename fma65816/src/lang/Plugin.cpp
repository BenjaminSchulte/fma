#include <fma/assem/RegisterOperand.hpp>
#include <fma/assem/ConstantNumberOperand.hpp>
#include <fma/assem/LocalAddressOperand.hpp>
#include <fma/assem/PcRelativeAddressOperand.hpp>
#include <fma/assem/FlagOperand.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/types/Class.hpp>
#include <fma/types/Decorator.hpp>
#include <fma/types/DecoratorContainer.hpp>
#include <fma/core/DataBlock.hpp>
#include <fma/core/Number.hpp>
#include <fma/core/TypedNumber.hpp>
#include <fma/core/String.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/InstanceContext.hpp>
#include <fma/interpret/Interpreter.hpp>
#include <fma/interpret/ProjectContext.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/Project.hpp>

#include <fma/instruct/Adc.hpp>
#include <fma/instruct/And.hpp>
#include <fma/instruct/Asl.hpp>
#include <fma/instruct/Bit.hpp>
#include <fma/instruct/Brk.hpp>
#include <fma/instruct/Call.hpp>
#include <fma/instruct/Cmp.hpp>
#include <fma/instruct/Dec.hpp>
#include <fma/instruct/Eor.hpp>
#include <fma/instruct/Inc.hpp>
#include <fma/instruct/Jmp.hpp>
#include <fma/instruct/Jz.hpp>
#include <fma/instruct/Jnz.hpp>
#include <fma/instruct/Js.hpp>
#include <fma/instruct/Jns.hpp>
#include <fma/instruct/Jc.hpp>
#include <fma/instruct/Jnc.hpp>
#include <fma/instruct/Jo.hpp>
#include <fma/instruct/Jno.hpp>
#include <fma/instruct/Lsr.hpp>
#include <fma/instruct/Or.hpp>
#include <fma/instruct/Rol.hpp>
#include <fma/instruct/Ror.hpp>
#include <fma/instruct/Rts.hpp>
#include <fma/instruct/Rtl.hpp>
#include <fma/instruct/Sbc.hpp>
#include <fma/instruct/Andnot.hpp>
#include <fma/instruct/Mov.hpp>
#include <fma/instruct/Nop.hpp>
#include <fma/instruct/Push.hpp>
#include <fma/instruct/Pop.hpp>
#include <fma/instruct/Stp.hpp>
#include <fma/instruct/Trb.hpp>
#include <fma/instruct/Tsb.hpp>
#include <fma/instruct/Rti.hpp>
#include <fma/instruct/Xchg.hpp>
#include <fma/instruct/Wai.hpp>
#include <lang/MoveBlock.hpp>

#include <lang/Plugin.hpp>
#include <lang/InstructionArguments.hpp>
#include <lang/Register.hpp>
#include <fma/plugin/MemoryPluginAdapter.hpp>

#include <iostream>

using namespace FMA;
using namespace FMA::assem;
using namespace FMA::types;
using namespace FMA::interpret;
using namespace FMA65816::lang;
using namespace std::placeholders;

// ----------------------------------------------------------------------------
LanguagePlugin::LanguagePlugin(Project *project)
  : FMA::plugin::LanguagePlugin(project)
{
}

#define INSTRUCTION_START(name) \
  proto->setMember(name, TypePtr(new InternalFunctionValue(name, [](const ContextPtr &context, const GroupedParameterList &params) -> ResultPtr { \
    plugin::MemoryBlock *block = core::DataBlockClass::memoryBlock(context); \
    if (block == NULL) { \
      context->log().error() << "Could not access memory block"; \
      return ResultPtr(new Result()); \
    }

#define INSTRUCTION_START_VARIANTS() \
    InstructionArguments args(context, context->getProject(), params); \
    if (!args.isValid()) {

#define INSTRUCTION(name) \
    INSTRUCTION_START(name) \
    block->setLastIsReturn(false); \
    INSTRUCTION_START_VARIANTS()

#define INSTRUCTION_TYPE_HINT(name, registerName) \
    INSTRUCTION_START(name) \
    block->setLastIsReturn(false); \
    testTypeHinting(context, params, registerName); \
    INSTRUCTION_START_VARIANTS()

#define INSTRUCTION_TYPE_HINT_ANY(name) \
    INSTRUCTION_START(name) \
    block->setLastIsReturn(false); \
    testTypeHinting(context, params); \
    INSTRUCTION_START_VARIANTS()

#define VARIANT(name) \
    } else if (args.name()) {

#define RETURN_INSTRUCTION(name) \
    INSTRUCTION_START(name) \
    block->setLastIsReturn(true); \
    INSTRUCTION_START_VARIANTS()

#define TRANSFER_INSTRUCTION(name, from, to) \
    INSTRUCTION_START(name) \
    block->setLastIsReturn(false); \
    transferRegisterType(context, from, to); \
    INSTRUCTION_START_VARIANTS()

#define CREATE(object) \
      Instruction *__inst = object; \
      block->write(__inst); \
      return ResultPtr(new Result());

#define END_INSTRUCTION(name) \
    } \
    context->log().error() << "Invalid parameters for " << name << " instruction"; \
    return ResultPtr(new Result()); \
  }))); \
  proto->setMember("::" name, proto->getMember(name))

// ----------------------------------------------------------------------------
bool LanguagePlugin::initialize() {
  const RootModulePtr &root = project->root();
  const ClassPtr &klass = root->getMember("Class")->asClass();
  const ClassPtr &dataBlock = root->getMember("DataBlock")->asClass();
  const ClassPtr &compiler = root->getMember("Compiler")->asClass();
  const ClassPtr &number = root->getMember("Number")->asClass();
  const ClassPtr &typedNumber = root->getMember("TypedNumber")->asClass();
  const ClassPtr &memoryVariable = root->getMember("MemoryVariable")->asClass();
  const ClassPtr &function = root->getMember("Function")->asClass();
  const ClassPtr &symbolRef = root->getMember("SymbolReference")->asClass();
  if (!dataBlock || !klass || !number || !typedNumber || !compiler || !function || !memoryVariable || !symbolRef) {
    project->log().error() << "Could not find DataBlock class";
    return false;
  }

  ClassPrototypePtr proto(dataBlock->getPrototype());
  INSTRUCTION("ADC");
    VARIANT(absolute)      CREATE(new instruct::ADC(new RegisterOperand("A"), args.createOperand()))
    VARIANT(immediate)
      Instruction *__inst = new instruct::ADC(new RegisterOperand("A"), args.createOperand());
      __inst->setAttribute("immediateSize", LanguagePlugin::getImmediateSize(context, "A"));
      block->write(__inst);
      return ResultPtr(new Result());

    VARIANT(directPage)    CREATE(new instruct::ADC(new RegisterOperand("A"), args.createOperand()))
    VARIANT(directPageX)   CREATE(new instruct::ADC(new RegisterOperand("A"), args.createOperand()))
    VARIANT(longAddress)   CREATE(new instruct::ADC(new RegisterOperand("A"), args.createOperand()))
    VARIANT(indirect)      CREATE(new instruct::ADC(new RegisterOperand("A"), args.createOperand()))
    VARIANT(longIndirect)  CREATE(new instruct::ADC(new RegisterOperand("A"), args.createOperand()))
    VARIANT(indirectY)     CREATE(new instruct::ADC(new RegisterOperand("A"), args.createOperand()))
    VARIANT(indirectX)     CREATE(new instruct::ADC(new RegisterOperand("A"), args.createOperand()))
    VARIANT(longIndirectY) CREATE(new instruct::ADC(new RegisterOperand("A"), args.createOperand()))
    VARIANT(absoluteX)     CREATE(new instruct::ADC(new RegisterOperand("A"), args.createOperand()))
    VARIANT(absoluteS)     CREATE(new instruct::ADC(new RegisterOperand("A"), args.createOperand()))
    VARIANT(absoluteY)     CREATE(new instruct::ADC(new RegisterOperand("A"), args.createOperand()))
    VARIANT(absoluteLongX) CREATE(new instruct::ADC(new RegisterOperand("A"), args.createOperand()))
  END_INSTRUCTION("ADC");
  INSTRUCTION("AND");
    VARIANT(absolute)      CREATE(new instruct::AND(new RegisterOperand("A"), args.createOperand()))
    VARIANT(immediate)
      Instruction *__inst = new instruct::AND(new RegisterOperand("A"), args.createOperand());
      __inst->setAttribute("immediateSize", LanguagePlugin::getImmediateSize(context, "A"));
      block->write(__inst);
      return ResultPtr(new Result());

    VARIANT(directPage)    CREATE(new instruct::AND(new RegisterOperand("A"), args.createOperand()))
    VARIANT(directPageX)   CREATE(new instruct::AND(new RegisterOperand("A"), args.createOperand()))
    VARIANT(longAddress)   CREATE(new instruct::AND(new RegisterOperand("A"), args.createOperand()))
    VARIANT(indirect)      CREATE(new instruct::AND(new RegisterOperand("A"), args.createOperand()))
    VARIANT(longIndirect)  CREATE(new instruct::AND(new RegisterOperand("A"), args.createOperand()))
    VARIANT(indirectY)     CREATE(new instruct::AND(new RegisterOperand("A"), args.createOperand()))
    VARIANT(indirectX)     CREATE(new instruct::AND(new RegisterOperand("A"), args.createOperand()))
    VARIANT(longIndirectY) CREATE(new instruct::AND(new RegisterOperand("A"), args.createOperand()))
    VARIANT(absoluteX)     CREATE(new instruct::AND(new RegisterOperand("A"), args.createOperand()))
    VARIANT(absoluteS)     CREATE(new instruct::AND(new RegisterOperand("A"), args.createOperand()))
    VARIANT(absoluteY)     CREATE(new instruct::AND(new RegisterOperand("A"), args.createOperand()))
    VARIANT(absoluteLongX) CREATE(new instruct::AND(new RegisterOperand("A"), args.createOperand()))
  END_INSTRUCTION("AND");
  INSTRUCTION("ASL");
    VARIANT(implicit)      CREATE(new instruct::ASL(new RegisterOperand("A"), new ConstantNumberOperand(1)))
    VARIANT(directPage)    CREATE(new instruct::ASL(args.createOperand(), new ConstantNumberOperand(1)))
    VARIANT(registerA)     CREATE(new instruct::ASL(args.createOperand(), new ConstantNumberOperand(1)))
    VARIANT(absolute)      CREATE(new instruct::ASL(args.createOperand(), new ConstantNumberOperand(1)))
    VARIANT(directPageX)   CREATE(new instruct::ASL(args.createOperand(), new ConstantNumberOperand(1)))
    VARIANT(absoluteX)     CREATE(new instruct::ASL(args.createOperand(), new ConstantNumberOperand(1)))
  END_INSTRUCTION("ASL");
  INSTRUCTION("BIT");
    VARIANT(immediate)
      Instruction *__inst = new instruct::BIT(new RegisterOperand("A"), args.createOperand());
      __inst->setAttribute("immediateSize", LanguagePlugin::getImmediateSize(context, "A"));
      block->write(__inst);
      return ResultPtr(new Result());
    VARIANT(directPage)    CREATE(new instruct::BIT(new RegisterOperand("A"), args.createOperand()))
    VARIANT(absolute)      CREATE(new instruct::BIT(new RegisterOperand("A"), args.createOperand()))
    VARIANT(directPageX)   CREATE(new instruct::BIT(new RegisterOperand("A"), args.createOperand()))
    VARIANT(absoluteX)     CREATE(new instruct::BIT(new RegisterOperand("A"), args.createOperand()))
  END_INSTRUCTION("BIT");
  RETURN_INSTRUCTION("BRA");
    VARIANT(absolute)      CREATE(new instruct::JMP(new PcRelativeAddressOperand(args.getLeft()->createValueOperand())));
  END_INSTRUCTION("BRA");
  INSTRUCTION("BCC");
    VARIANT(absolute)      CREATE(new instruct::JNC(new PcRelativeAddressOperand(args.getLeft()->createValueOperand())));
  END_INSTRUCTION("BCC");
  INSTRUCTION("BLT");
    VARIANT(absolute)      CREATE(new instruct::JNC(new PcRelativeAddressOperand(args.getLeft()->createValueOperand())));
  END_INSTRUCTION("BLT");
  INSTRUCTION("BGE");
    VARIANT(absolute)      CREATE(new instruct::JC(new PcRelativeAddressOperand(args.getLeft()->createValueOperand())));
  END_INSTRUCTION("BGE");
  INSTRUCTION("BCS");
    VARIANT(absolute)      CREATE(new instruct::JC(new PcRelativeAddressOperand(args.getLeft()->createValueOperand())));
  END_INSTRUCTION("BCS");
  INSTRUCTION("BEQ");
    VARIANT(absolute)      CREATE(new instruct::JZ(new PcRelativeAddressOperand(args.getLeft()->createValueOperand())));
  END_INSTRUCTION("BEQ");
  INSTRUCTION("BZE");
    VARIANT(absolute)      CREATE(new instruct::JZ(new PcRelativeAddressOperand(args.getLeft()->createValueOperand())));
  END_INSTRUCTION("BZE");
  INSTRUCTION("BNE");
    VARIANT(absolute)      CREATE(new instruct::JNZ(new PcRelativeAddressOperand(args.getLeft()->createValueOperand())));
  END_INSTRUCTION("BNE");
  INSTRUCTION("BNZ");
    VARIANT(absolute)      CREATE(new instruct::JNZ(new PcRelativeAddressOperand(args.getLeft()->createValueOperand())));
  END_INSTRUCTION("BNZ");
  INSTRUCTION("BMI");
    VARIANT(absolute)      CREATE(new instruct::JS(new PcRelativeAddressOperand(args.getLeft()->createValueOperand())));
  END_INSTRUCTION("BMI");
  INSTRUCTION("BPL");
    VARIANT(absolute)      CREATE(new instruct::JNS(new PcRelativeAddressOperand(args.getLeft()->createValueOperand())));
  END_INSTRUCTION("BPL");
  INSTRUCTION("BVC");
    VARIANT(absolute)      CREATE(new instruct::JNO(new PcRelativeAddressOperand(args.getLeft()->createValueOperand())));
  END_INSTRUCTION("BVC");
  INSTRUCTION("BVS");
    VARIANT(absolute)      CREATE(new instruct::JO(new PcRelativeAddressOperand(args.getLeft()->createValueOperand())));
  END_INSTRUCTION("BVS");
  INSTRUCTION("BRK");
    VARIANT(implicit) CREATE(new instruct::BRK())
  END_INSTRUCTION("BRK");
  INSTRUCTION("CLC");
    VARIANT(implicit) CREATE(new instruct::ANDNOT(new RegisterOperand("P"), new ConstantNumberOperand(0x01)))
  END_INSTRUCTION("CLC");
  INSTRUCTION("CLD");
    VARIANT(implicit) CREATE(new instruct::ANDNOT(new RegisterOperand("P"), new ConstantNumberOperand(0x08)))
  END_INSTRUCTION("CLD");
  INSTRUCTION("CLI");
    VARIANT(implicit) CREATE(new instruct::ANDNOT(new RegisterOperand("P"), new ConstantNumberOperand(0x04)))
  END_INSTRUCTION("CLI");
  INSTRUCTION("CLV");
    VARIANT(implicit) CREATE(new instruct::ANDNOT(new RegisterOperand("P"), new ConstantNumberOperand(0x40)))
  END_INSTRUCTION("CLV");
  INSTRUCTION("CMP");
    VARIANT(absolute)      CREATE(new instruct::CMP(new RegisterOperand("A"), args.createOperand()))
    VARIANT(immediate)
      Instruction *__inst = new instruct::CMP(new RegisterOperand("A"), args.createOperand());
      __inst->setAttribute("immediateSize", LanguagePlugin::getImmediateSize(context, "A"));
      block->write(__inst);
      return ResultPtr(new Result());

    VARIANT(directPage)    CREATE(new instruct::CMP(new RegisterOperand("A"), args.createOperand()))
    VARIANT(directPageX)   CREATE(new instruct::CMP(new RegisterOperand("A"), args.createOperand()))
    VARIANT(longAddress)   CREATE(new instruct::CMP(new RegisterOperand("A"), args.createOperand()))
    VARIANT(indirect)      CREATE(new instruct::CMP(new RegisterOperand("A"), args.createOperand()))
    VARIANT(longIndirect)  CREATE(new instruct::CMP(new RegisterOperand("A"), args.createOperand()))
    VARIANT(indirectY)     CREATE(new instruct::CMP(new RegisterOperand("A"), args.createOperand()))
    VARIANT(indirectX)     CREATE(new instruct::CMP(new RegisterOperand("A"), args.createOperand()))
    VARIANT(longIndirectY) CREATE(new instruct::CMP(new RegisterOperand("A"), args.createOperand()))
    VARIANT(absoluteX)     CREATE(new instruct::CMP(new RegisterOperand("A"), args.createOperand()))
    VARIANT(absoluteS)     CREATE(new instruct::CMP(new RegisterOperand("A"), args.createOperand()))
    VARIANT(absoluteY)     CREATE(new instruct::CMP(new RegisterOperand("A"), args.createOperand()))
    VARIANT(absoluteLongX) CREATE(new instruct::CMP(new RegisterOperand("A"), args.createOperand()))
  END_INSTRUCTION("CMP");
  INSTRUCTION("CMP8");
    VARIANT(immediate)
      Instruction *__inst = new instruct::CMP(new RegisterOperand("A"), args.createOperand());
      __inst->setAttribute("immediateSize", (uint8_t)8);
      block->write(__inst);
      return ResultPtr(new Result());
  END_INSTRUCTION("CMP8");
  INSTRUCTION("CMP16");
    VARIANT(immediate)
      Instruction *__inst = new instruct::CMP(new RegisterOperand("A"), args.createOperand());
      __inst->setAttribute("immediateSize", (uint8_t)16);
      block->write(__inst);
      return ResultPtr(new Result());
  END_INSTRUCTION("CMP16");
  INSTRUCTION("CPX");
    VARIANT(immediate)
      Instruction *__inst = new instruct::CMP(new RegisterOperand("X"), args.createOperand());
      __inst->setAttribute("immediateSize", LanguagePlugin::getImmediateSize(context, "XY"));
      block->write(__inst);
      return ResultPtr(new Result());

    VARIANT(directPage)    CREATE(new instruct::CMP(new RegisterOperand("X"), args.createOperand()))
    VARIANT(absolute)      CREATE(new instruct::CMP(new RegisterOperand("X"), args.createOperand()))
  END_INSTRUCTION("CPX");
  INSTRUCTION("CPY");
    VARIANT(immediate)
      Instruction *__inst = new instruct::CMP(new RegisterOperand("Y"), args.createOperand());
      __inst->setAttribute("immediateSize", LanguagePlugin::getImmediateSize(context, "XY"));
      block->write(__inst);
      return ResultPtr(new Result());

    VARIANT(directPage)    CREATE(new instruct::CMP(new RegisterOperand("Y"), args.createOperand()))
    VARIANT(absolute)      CREATE(new instruct::CMP(new RegisterOperand("Y"), args.createOperand()))
  END_INSTRUCTION("CPY");
  INSTRUCTION("DEC");
    VARIANT(implicit)      CREATE(new instruct::DEC(new RegisterOperand("A")))
    VARIANT(registerA)     CREATE(new instruct::DEC(args.createOperand()))
    VARIANT(directPage)    CREATE(new instruct::DEC(args.createOperand()))
    VARIANT(absolute)      CREATE(new instruct::DEC(args.createOperand()))
    VARIANT(directPageX)   CREATE(new instruct::DEC(args.createOperand()))
    VARIANT(absoluteX)     CREATE(new instruct::DEC(args.createOperand()))
    VARIANT(registerX)     CREATE(new instruct::DEC(args.createOperand()))
    VARIANT(registerY)     CREATE(new instruct::DEC(args.createOperand()))
  END_INSTRUCTION("DEC");
  INSTRUCTION("DEX");
    VARIANT(implicit)      CREATE(new instruct::DEC(new RegisterOperand("X")))
  END_INSTRUCTION("DEX");
  INSTRUCTION("DEY");
    VARIANT(implicit)      CREATE(new instruct::DEC(new RegisterOperand("Y")))
  END_INSTRUCTION("DEY");
  INSTRUCTION("EOR");
    VARIANT(absolute)      CREATE(new instruct::EOR(new RegisterOperand("A"), args.createOperand()))
    VARIANT(immediate)
      Instruction *__inst = new instruct::EOR(new RegisterOperand("A"), args.createOperand());
      __inst->setAttribute("immediateSize", LanguagePlugin::getImmediateSize(context, "A"));
      block->write(__inst);
      return ResultPtr(new Result());

    VARIANT(directPage)    CREATE(new instruct::EOR(new RegisterOperand("A"), args.createOperand()))
    VARIANT(directPageX)   CREATE(new instruct::EOR(new RegisterOperand("A"), args.createOperand()))
    VARIANT(longAddress)   CREATE(new instruct::EOR(new RegisterOperand("A"), args.createOperand()))
    VARIANT(indirect)      CREATE(new instruct::EOR(new RegisterOperand("A"), args.createOperand()))
    VARIANT(longIndirect)  CREATE(new instruct::EOR(new RegisterOperand("A"), args.createOperand()))
    VARIANT(indirectY)     CREATE(new instruct::EOR(new RegisterOperand("A"), args.createOperand()))
    VARIANT(indirectX)     CREATE(new instruct::EOR(new RegisterOperand("A"), args.createOperand()))
    VARIANT(longIndirectY) CREATE(new instruct::EOR(new RegisterOperand("A"), args.createOperand()))
    VARIANT(absoluteX)     CREATE(new instruct::EOR(new RegisterOperand("A"), args.createOperand()))
    VARIANT(absoluteS)     CREATE(new instruct::EOR(new RegisterOperand("A"), args.createOperand()))
    VARIANT(absoluteY)     CREATE(new instruct::EOR(new RegisterOperand("A"), args.createOperand()))
    VARIANT(absoluteLongX) CREATE(new instruct::EOR(new RegisterOperand("A"), args.createOperand()))
  END_INSTRUCTION("EOR");
  INSTRUCTION("INC");
    VARIANT(immediate)     CREATE(new instruct::INC(new RegisterOperand("A")))
    VARIANT(registerA)     CREATE(new instruct::INC(args.createOperand()))
    VARIANT(directPage)    CREATE(new instruct::INC(args.createOperand()))
    VARIANT(absolute)      CREATE(new instruct::INC(args.createOperand()))
    VARIANT(directPageX)   CREATE(new instruct::INC(args.createOperand()))
    VARIANT(absoluteX)     CREATE(new instruct::INC(args.createOperand()))
    VARIANT(registerX)     CREATE(new instruct::INC(args.createOperand()))
    VARIANT(registerY)     CREATE(new instruct::INC(args.createOperand()))
  END_INSTRUCTION("INC");
  INSTRUCTION("INX");
    VARIANT(implicit)      CREATE(new instruct::INC(new RegisterOperand("X")))
  END_INSTRUCTION("INX");
  INSTRUCTION("INY");
    VARIANT(implicit)      CREATE(new instruct::INC(new RegisterOperand("Y")))
  END_INSTRUCTION("INY");
  RETURN_INSTRUCTION("JML");
    VARIANT(absolute)      CREATE(new instruct::JMP(args.getLeft()->createValueOperand()));
    VARIANT(longAddress)   CREATE(new instruct::JMP(args.getLeft()->createValueOperand()));
  END_INSTRUCTION("JML");
  INSTRUCTION_TYPE_HINT_ANY("JSR");
    VARIANT(absolute)      CREATE(new instruct::CALL(new LocalAddressOperand(args.getLeft()->createValueOperand())));
    VARIANT(longAddress)   CREATE(new instruct::CALL(args.getLeft()->createValueOperand()));
    VARIANT(indirectX)     CREATE(new instruct::CALL(args.createOperand()))
  END_INSTRUCTION("JSR");
  INSTRUCTION_TYPE_HINT_ANY("JSL");
    VARIANT(absolute)      CREATE(new instruct::CALL(args.getLeft()->createValueOperand()));
    VARIANT(longAddress)   CREATE(new instruct::CALL(args.getLeft()->createValueOperand()));
  END_INSTRUCTION("JSL");
  RETURN_INSTRUCTION("JMP");
    VARIANT(absolute)      CREATE(new instruct::JMP(new LocalAddressOperand(args.getLeft()->createValueOperand())));
    VARIANT(longAddress)   CREATE(new instruct::JMP(args.getLeft()->createValueOperand()));
    VARIANT(indirectX)     CREATE(new instruct::JMP(args.createOperand()))
  END_INSTRUCTION("JMP");
  INSTRUCTION_TYPE_HINT("LDA", "A");
    VARIANT(absolute)      CREATE(new instruct::MOV(new RegisterOperand("A"), args.createOperand()))
    VARIANT(immediate)
      Instruction *__inst = new instruct::MOV(new RegisterOperand("A"), args.createOperand());
      __inst->setAttribute("immediateSize", LanguagePlugin::getImmediateSize(context, "A"));
      block->write(__inst);
      return ResultPtr(new Result());

    VARIANT(directPage)    CREATE(new instruct::MOV(new RegisterOperand("A"), args.createOperand()))
    VARIANT(directPageX)   CREATE(new instruct::MOV(new RegisterOperand("A"), args.createOperand()))
    VARIANT(longAddress)   CREATE(new instruct::MOV(new RegisterOperand("A"), args.createOperand()))
    VARIANT(indirect)      CREATE(new instruct::MOV(new RegisterOperand("A"), args.createOperand()))
    VARIANT(longIndirect)  CREATE(new instruct::MOV(new RegisterOperand("A"), args.createOperand()))
    VARIANT(indirectY)     CREATE(new instruct::MOV(new RegisterOperand("A"), args.createOperand()))
    VARIANT(indirectX)     CREATE(new instruct::MOV(new RegisterOperand("A"), args.createOperand()))
    VARIANT(longIndirectY) CREATE(new instruct::MOV(new RegisterOperand("A"), args.createOperand()))
    VARIANT(absoluteX)     CREATE(new instruct::MOV(new RegisterOperand("A"), args.createOperand()))
    VARIANT(absoluteS)     CREATE(new instruct::MOV(new RegisterOperand("A"), args.createOperand()))
    VARIANT(absoluteY)     CREATE(new instruct::MOV(new RegisterOperand("A"), args.createOperand()))
    VARIANT(absoluteLongX) CREATE(new instruct::MOV(new RegisterOperand("A"), args.createOperand()))
  END_INSTRUCTION("LDA");
  INSTRUCTION_TYPE_HINT("LDA8", "A");
    VARIANT(immediate)
      Instruction *__inst = new instruct::MOV(new RegisterOperand("A"), args.createOperand());
      __inst->setAttribute("immediateSize", (uint8_t)8);
      block->write(__inst);
      return ResultPtr(new Result());
  END_INSTRUCTION("LDA8");
  INSTRUCTION_TYPE_HINT("LDA16", "A");
    VARIANT(immediate)
      Instruction *__inst = new instruct::MOV(new RegisterOperand("A"), args.createOperand());
      __inst->setAttribute("immediateSize", (uint8_t)16);
      block->write(__inst);
      return ResultPtr(new Result());
  END_INSTRUCTION("LDA16");
  INSTRUCTION_TYPE_HINT("LDX", "X");
    VARIANT(immediate)
      Instruction *__inst = new instruct::MOV(new RegisterOperand("X"), args.createOperand());
      __inst->setAttribute("immediateSize", LanguagePlugin::getImmediateSize(context, "XY"));
      block->write(__inst);
      return ResultPtr(new Result());
    
    VARIANT(directPage)    CREATE(new instruct::MOV(new RegisterOperand("X"), args.createOperand()))
    VARIANT(absolute)      CREATE(new instruct::MOV(new RegisterOperand("X"), args.createOperand()))
    VARIANT(directPageY)   CREATE(new instruct::MOV(new RegisterOperand("X"), args.createOperand()))
    VARIANT(absoluteY)     CREATE(new instruct::MOV(new RegisterOperand("X"), args.createOperand()))
  END_INSTRUCTION("LDX");
  INSTRUCTION_TYPE_HINT("LDX8", "X");
    VARIANT(immediate)
      Instruction *__inst = new instruct::MOV(new RegisterOperand("X"), args.createOperand());
      __inst->setAttribute("immediateSize", (uint8_t)8);
      block->write(__inst);
      return ResultPtr(new Result());
  END_INSTRUCTION("LDX8");
  INSTRUCTION_TYPE_HINT("LDX16", "X");
    VARIANT(immediate)
      Instruction *__inst = new instruct::MOV(new RegisterOperand("X"), args.createOperand());
      __inst->setAttribute("immediateSize", (uint8_t)16);
      block->write(__inst);
      return ResultPtr(new Result());
  END_INSTRUCTION("LDX16");
  INSTRUCTION_TYPE_HINT("LDY", "Y");
    VARIANT(immediate)
      Instruction *__inst = new instruct::MOV(new RegisterOperand("Y"), args.createOperand());
      __inst->setAttribute("immediateSize", LanguagePlugin::getImmediateSize(context, "XY"));
      block->write(__inst);
      return ResultPtr(new Result());
    
    VARIANT(directPage)    CREATE(new instruct::MOV(new RegisterOperand("Y"), args.createOperand()))
    VARIANT(absolute)      CREATE(new instruct::MOV(new RegisterOperand("Y"), args.createOperand()))
    VARIANT(directPageX)   CREATE(new instruct::MOV(new RegisterOperand("Y"), args.createOperand()))
    VARIANT(absoluteX)     CREATE(new instruct::MOV(new RegisterOperand("Y"), args.createOperand()))
  END_INSTRUCTION("LDY");
  INSTRUCTION_TYPE_HINT("LDY8", "Y");
    VARIANT(immediate)
      Instruction *__inst = new instruct::MOV(new RegisterOperand("Y"), args.createOperand());
      __inst->setAttribute("immediateSize", (uint8_t)8);
      block->write(__inst);
      return ResultPtr(new Result());
  END_INSTRUCTION("LDY8");
  INSTRUCTION_TYPE_HINT("LDY16", "Y");
    VARIANT(immediate)
      Instruction *__inst = new instruct::MOV(new RegisterOperand("Y"), args.createOperand());
      __inst->setAttribute("immediateSize", (uint8_t)16);
      block->write(__inst);
      return ResultPtr(new Result());
  END_INSTRUCTION("LDY16");
  INSTRUCTION("LSR");
    VARIANT(implicit)      CREATE(new instruct::LSR(new RegisterOperand("A"), new ConstantNumberOperand(1)))
    VARIANT(directPage)    CREATE(new instruct::LSR(args.createOperand(), new ConstantNumberOperand(1)))
    VARIANT(registerA)     CREATE(new instruct::LSR(args.createOperand(), new ConstantNumberOperand(1)))
    VARIANT(absolute)      CREATE(new instruct::LSR(args.createOperand(), new ConstantNumberOperand(1)))
    VARIANT(directPageX)   CREATE(new instruct::LSR(args.createOperand(), new ConstantNumberOperand(1)))
    VARIANT(absoluteX)     CREATE(new instruct::LSR(args.createOperand(), new ConstantNumberOperand(1)))
  END_INSTRUCTION("LSR");
  INSTRUCTION("MVN");
    VARIANT(move)          CREATE(new MoveBlock(false, args.getLeft()->createValueOperand(), args.getRight()->createValueOperand()))
  END_INSTRUCTION("MVN");
  INSTRUCTION("MVP");
    VARIANT(move)          CREATE(new MoveBlock(true, args.getLeft()->createValueOperand(), args.getRight()->createValueOperand()))
  END_INSTRUCTION("MVP");
  INSTRUCTION("NOP");
    VARIANT(implicit) CREATE(new instruct::NOP());
  END_INSTRUCTION("NOP");
  INSTRUCTION("SBC");
    VARIANT(absolute)      CREATE(new instruct::SBC(new RegisterOperand("A"), args.createOperand()))
    VARIANT(immediate)
      Instruction *__inst = new instruct::SBC(new RegisterOperand("A"), args.createOperand());
      __inst->setAttribute("immediateSize", LanguagePlugin::getImmediateSize(context, "A"));
      block->write(__inst);
      return ResultPtr(new Result());

    VARIANT(directPage)    CREATE(new instruct::SBC(new RegisterOperand("A"), args.createOperand()))
    VARIANT(directPageX)   CREATE(new instruct::SBC(new RegisterOperand("A"), args.createOperand()))
    VARIANT(longAddress)   CREATE(new instruct::SBC(new RegisterOperand("A"), args.createOperand()))
    VARIANT(indirect)      CREATE(new instruct::SBC(new RegisterOperand("A"), args.createOperand()))
    VARIANT(longIndirect)  CREATE(new instruct::SBC(new RegisterOperand("A"), args.createOperand()))
    VARIANT(indirectY)     CREATE(new instruct::SBC(new RegisterOperand("A"), args.createOperand()))
    VARIANT(indirectX)     CREATE(new instruct::SBC(new RegisterOperand("A"), args.createOperand()))
    VARIANT(longIndirectY) CREATE(new instruct::SBC(new RegisterOperand("A"), args.createOperand()))
    VARIANT(absoluteX)     CREATE(new instruct::SBC(new RegisterOperand("A"), args.createOperand()))
    VARIANT(absoluteS)     CREATE(new instruct::SBC(new RegisterOperand("A"), args.createOperand()))
    VARIANT(absoluteY)     CREATE(new instruct::SBC(new RegisterOperand("A"), args.createOperand()))
    VARIANT(absoluteLongX) CREATE(new instruct::SBC(new RegisterOperand("A"), args.createOperand()))
  END_INSTRUCTION("SBC");
  INSTRUCTION("ROL");
    VARIANT(implicit)      CREATE(new instruct::ROL(new RegisterOperand("A"), new ConstantNumberOperand(1)))
    VARIANT(directPage)    CREATE(new instruct::ROL(args.createOperand(), new ConstantNumberOperand(1)))
    VARIANT(registerA)     CREATE(new instruct::ROL(args.createOperand(), new ConstantNumberOperand(1)))
    VARIANT(absolute)      CREATE(new instruct::ROL(args.createOperand(), new ConstantNumberOperand(1)))
    VARIANT(directPageX)   CREATE(new instruct::ROL(args.createOperand(), new ConstantNumberOperand(1)))
    VARIANT(absoluteX)     CREATE(new instruct::ROL(args.createOperand(), new ConstantNumberOperand(1)))
  END_INSTRUCTION("ROL");
  INSTRUCTION("ROR");
    VARIANT(implicit)      CREATE(new instruct::ROR(new RegisterOperand("A"), new ConstantNumberOperand(1)))
    VARIANT(directPage)    CREATE(new instruct::ROR(args.createOperand(), new ConstantNumberOperand(1)))
    VARIANT(registerA)     CREATE(new instruct::ROR(args.createOperand(), new ConstantNumberOperand(1)))
    VARIANT(absolute)      CREATE(new instruct::ROR(args.createOperand(), new ConstantNumberOperand(1)))
    VARIANT(directPageX)   CREATE(new instruct::ROR(args.createOperand(), new ConstantNumberOperand(1)))
    VARIANT(absoluteX)     CREATE(new instruct::ROR(args.createOperand(), new ConstantNumberOperand(1)))
  END_INSTRUCTION("ROR");
  RETURN_INSTRUCTION("RTI");
    VARIANT(implicit)      CREATE(new instruct::RTI());
  END_INSTRUCTION("RTI");
  RETURN_INSTRUCTION("RTS");
    VARIANT(implicit)      CREATE(new instruct::RTS());
  END_INSTRUCTION("RTS");
  RETURN_INSTRUCTION("RTL");
    VARIANT(implicit)      CREATE(new instruct::RTL());
  END_INSTRUCTION("RTL");
  RETURN_INSTRUCTION("TSB");
    VARIANT(directPage)    CREATE(new instruct::TSB(args.createOperand(), new RegisterOperand("A")));
    VARIANT(absolute)      CREATE(new instruct::TSB(args.createOperand(), new RegisterOperand("A")));
  END_INSTRUCTION("TSB");
  RETURN_INSTRUCTION("TRB");
    VARIANT(directPage)    CREATE(new instruct::TRB(args.createOperand(), new RegisterOperand("A")));
    VARIANT(absolute)      CREATE(new instruct::TRB(args.createOperand(), new RegisterOperand("A")));
  END_INSTRUCTION("TRB");
  INSTRUCTION("ORA");
    VARIANT(absolute)      CREATE(new instruct::OR(new RegisterOperand("A"), args.createOperand()))
    VARIANT(immediate)
      Instruction *__inst = new instruct::OR(new RegisterOperand("A"), args.createOperand());
      __inst->setAttribute("immediateSize", LanguagePlugin::getImmediateSize(context, "A"));
      block->write(__inst);
      return ResultPtr(new Result());

    VARIANT(directPage)    CREATE(new instruct::OR(new RegisterOperand("A"), args.createOperand()))
    VARIANT(directPageX)   CREATE(new instruct::OR(new RegisterOperand("A"), args.createOperand()))
    VARIANT(longAddress)   CREATE(new instruct::OR(new RegisterOperand("A"), args.createOperand()))
    VARIANT(indirect)      CREATE(new instruct::OR(new RegisterOperand("A"), args.createOperand()))
    VARIANT(longIndirect)  CREATE(new instruct::OR(new RegisterOperand("A"), args.createOperand()))
    VARIANT(indirectY)     CREATE(new instruct::OR(new RegisterOperand("A"), args.createOperand()))
    VARIANT(indirectX)     CREATE(new instruct::OR(new RegisterOperand("A"), args.createOperand()))
    VARIANT(longIndirectY) CREATE(new instruct::OR(new RegisterOperand("A"), args.createOperand()))
    VARIANT(absoluteX)     CREATE(new instruct::OR(new RegisterOperand("A"), args.createOperand()))
    VARIANT(absoluteS)     CREATE(new instruct::OR(new RegisterOperand("A"), args.createOperand()))
    VARIANT(absoluteY)     CREATE(new instruct::OR(new RegisterOperand("A"), args.createOperand()))
    VARIANT(absoluteLongX) CREATE(new instruct::OR(new RegisterOperand("A"), args.createOperand()))
  END_INSTRUCTION("ORA");
  INSTRUCTION("PEA");
    VARIANT(immediate)
      Instruction *__inst = new instruct::PUSH(args.createOperand());
      __inst->setAttribute("immediateSize", (uint8_t)16);
      block->write(__inst);
      return ResultPtr(new Result());
  END_INSTRUCTION("PEA");
  INSTRUCTION("PEI");
    VARIANT(directPage)    CREATE(new instruct::PUSH(args.createOperand()))
  END_INSTRUCTION("PEI");
  INSTRUCTION("PHA");
    VARIANT(implicit)      CREATE(new instruct::PUSH(new RegisterOperand("A")))
  END_INSTRUCTION("PHA");
  INSTRUCTION("PHB");
    VARIANT(implicit)      CREATE(new instruct::PUSH(new RegisterOperand("DB")))
  END_INSTRUCTION("PHB");
  INSTRUCTION("PHD");
    VARIANT(implicit)      CREATE(new instruct::PUSH(new RegisterOperand("D")))
  END_INSTRUCTION("PHD");
  INSTRUCTION("PHK");
    VARIANT(implicit)      CREATE(new instruct::PUSH(new RegisterOperand("PB")))
  END_INSTRUCTION("PHK");
  INSTRUCTION("PHP");
    VARIANT(implicit)      CREATE(new instruct::PUSH(new RegisterOperand("P")))
  END_INSTRUCTION("PHP");
  INSTRUCTION("PHX");
    VARIANT(implicit)      CREATE(new instruct::PUSH(new RegisterOperand("X")))
  END_INSTRUCTION("PHX");
  INSTRUCTION("PHY");
    VARIANT(implicit)      CREATE(new instruct::PUSH(new RegisterOperand("Y")))
  END_INSTRUCTION("PHY");
  INSTRUCTION("PLA");
    VARIANT(implicit)      CREATE(new instruct::POP(new RegisterOperand("A")))
  END_INSTRUCTION("PLA");
  INSTRUCTION("PLB");
    VARIANT(implicit)      CREATE(new instruct::POP(new RegisterOperand("DB")))
  END_INSTRUCTION("PLB");
  INSTRUCTION("PLD");
    VARIANT(implicit)      CREATE(new instruct::POP(new RegisterOperand("D")))
  END_INSTRUCTION("PLD");
  INSTRUCTION("PLP");
    VARIANT(implicit)      CREATE(new instruct::POP(new RegisterOperand("P")))
  END_INSTRUCTION("PLP");
  INSTRUCTION("PLX");
    VARIANT(implicit)      CREATE(new instruct::POP(new RegisterOperand("X")))
  END_INSTRUCTION("PLX");
  INSTRUCTION("PLY");
    VARIANT(implicit)      CREATE(new instruct::POP(new RegisterOperand("Y")))
  END_INSTRUCTION("PLY");
  INSTRUCTION("REP");
    VARIANT(immediate)
    if (args.getLeft()->valueType == InstructionArgument::VALUE_NUMBER) {
      LanguagePlugin::resetFlagRegister(context, args.getLeft()->number);
    }
    CREATE(new instruct::ANDNOT(new RegisterOperand("P"), args.createLeftOperand()))
  END_INSTRUCTION("REP");
  INSTRUCTION("SEC");
    VARIANT(implicit)      CREATE(new instruct::OR(new RegisterOperand("P"), new ConstantNumberOperand(0x01)))
  END_INSTRUCTION("SEC");
  INSTRUCTION("SED");
    VARIANT(implicit)      CREATE(new instruct::OR(new RegisterOperand("P"), new ConstantNumberOperand(0x08)))
  END_INSTRUCTION("SED");
  INSTRUCTION("SEI");
    VARIANT(implicit)      CREATE(new instruct::OR(new RegisterOperand("P"), new ConstantNumberOperand(0x04)))
  END_INSTRUCTION("SEI");
  INSTRUCTION("SEP");
    VARIANT(immediate)
    if (args.getLeft()->valueType == InstructionArgument::VALUE_NUMBER) {
      LanguagePlugin::setFlagRegister(context, args.getLeft()->number);
    }
    CREATE(new instruct::OR(new RegisterOperand("P"), args.createLeftOperand()))
  END_INSTRUCTION("SEP");
  INSTRUCTION("STA");
    VARIANT(absolute)      CREATE(new instruct::MOV(args.createOperand(), new RegisterOperand("A")))
    VARIANT(directPage)    CREATE(new instruct::MOV(args.createOperand(), new RegisterOperand("A")))
    VARIANT(directPageX)   CREATE(new instruct::MOV(args.createOperand(), new RegisterOperand("A")))
    VARIANT(longAddress)   CREATE(new instruct::MOV(args.createOperand(), new RegisterOperand("A")))
    VARIANT(indirect)      CREATE(new instruct::MOV(args.createOperand(), new RegisterOperand("A")))
    VARIANT(longIndirect)  CREATE(new instruct::MOV(args.createOperand(), new RegisterOperand("A")))
    VARIANT(indirectY)     CREATE(new instruct::MOV(args.createOperand(), new RegisterOperand("A")))
    VARIANT(indirectX)     CREATE(new instruct::MOV(args.createOperand(), new RegisterOperand("A")))
    VARIANT(longIndirectY) CREATE(new instruct::MOV(args.createOperand(), new RegisterOperand("A")))
    VARIANT(absoluteX)     CREATE(new instruct::MOV(args.createOperand(), new RegisterOperand("A")))
    VARIANT(absoluteS)     CREATE(new instruct::MOV(args.createOperand(), new RegisterOperand("A")))
    VARIANT(absoluteY)     CREATE(new instruct::MOV(args.createOperand(), new RegisterOperand("A")))
    VARIANT(absoluteLongX) CREATE(new instruct::MOV(args.createOperand(), new RegisterOperand("A")))
  END_INSTRUCTION("STA");
  INSTRUCTION("STP");
    VARIANT(implicit)      CREATE(new instruct::STP())
  END_INSTRUCTION("STP");
  INSTRUCTION("STX");
    VARIANT(directPage)    CREATE(new instruct::MOV(args.createOperand(), new RegisterOperand("X")))
    VARIANT(absolute)      CREATE(new instruct::MOV(args.createOperand(), new RegisterOperand("X")))
    VARIANT(directPageY)   CREATE(new instruct::MOV(args.createOperand(), new RegisterOperand("X")))
  END_INSTRUCTION("STX");
  INSTRUCTION("STY");
    VARIANT(directPage)    CREATE(new instruct::MOV(args.createOperand(), new RegisterOperand("Y")))
    VARIANT(absolute)      CREATE(new instruct::MOV(args.createOperand(), new RegisterOperand("Y")))
    VARIANT(directPageX)   CREATE(new instruct::MOV(args.createOperand(), new RegisterOperand("Y")))
  END_INSTRUCTION("STY");
  INSTRUCTION("STZ");
    VARIANT(directPage)    CREATE(new instruct::MOV(args.createOperand(), new ConstantNumberOperand(0)))
    VARIANT(directPageX)   CREATE(new instruct::MOV(args.createOperand(), new ConstantNumberOperand(0)))
    VARIANT(absolute)      CREATE(new instruct::MOV(args.createOperand(), new ConstantNumberOperand(0)))
    VARIANT(absoluteX)     CREATE(new instruct::MOV(args.createOperand(), new ConstantNumberOperand(0)))
  END_INSTRUCTION("STZ");
  TRANSFER_INSTRUCTION("TAX", "A", "X");
    VARIANT(implicit) CREATE(new instruct::MOV(new RegisterOperand("X"), new RegisterOperand("A")))
  END_INSTRUCTION("TAX");
  TRANSFER_INSTRUCTION("TAY", "A", "Y");
    VARIANT(implicit) CREATE(new instruct::MOV(new RegisterOperand("Y"), new RegisterOperand("A")))
  END_INSTRUCTION("TAY");
  INSTRUCTION("TCD");
    VARIANT(implicit) CREATE(new instruct::MOV(new RegisterOperand("D"), new RegisterOperand("C")))
  END_INSTRUCTION("TCD");
  INSTRUCTION("TCS");
    VARIANT(implicit) CREATE(new instruct::MOV(new RegisterOperand("S"), new RegisterOperand("C")))
  END_INSTRUCTION("TCS");
  INSTRUCTION("TDC");
    VARIANT(implicit) CREATE(new instruct::MOV(new RegisterOperand("C"), new RegisterOperand("D")))
  END_INSTRUCTION("TDC");
  INSTRUCTION("TSC");
    VARIANT(implicit) CREATE(new instruct::MOV(new RegisterOperand("C"), new RegisterOperand("S")))
  END_INSTRUCTION("TSC");
  INSTRUCTION("TSX");
    VARIANT(implicit) CREATE(new instruct::MOV(new RegisterOperand("X"), new RegisterOperand("S")))
  END_INSTRUCTION("TSX");
  TRANSFER_INSTRUCTION("TXA", "X", "A");
    VARIANT(implicit) CREATE(new instruct::MOV(new RegisterOperand("A"), new RegisterOperand("X")))
  END_INSTRUCTION("TXA");
  INSTRUCTION("TXS");
    VARIANT(implicit) CREATE(new instruct::MOV(new RegisterOperand("S"), new RegisterOperand("X")))
  END_INSTRUCTION("TXS");
  TRANSFER_INSTRUCTION("TXY", "X", "Y");
    VARIANT(implicit) CREATE(new instruct::MOV(new RegisterOperand("Y"), new RegisterOperand("X")))
  END_INSTRUCTION("TXY");
  TRANSFER_INSTRUCTION("TYA", "Y", "A");
    VARIANT(implicit) CREATE(new instruct::MOV(new RegisterOperand("A"), new RegisterOperand("Y")))
  END_INSTRUCTION("TYA");
  TRANSFER_INSTRUCTION("TYX", "Y", "X");
    VARIANT(implicit) CREATE(new instruct::MOV(new RegisterOperand("X"), new RegisterOperand("Y")))
  END_INSTRUCTION("TYX");
  INSTRUCTION("XBA");
    VARIANT(implicit) CREATE(new instruct::XCHG(new RegisterOperand("B"), new RegisterOperand("A")))
  END_INSTRUCTION("XBA");
  INSTRUCTION("XCE");
    VARIANT(implicit) CREATE(new instruct::XCHG(new FlagOperand("E"), new FlagOperand("C")))
  END_INSTRUCTION("XCE");
  INSTRUCTION("WAI");
    VARIANT(implicit) CREATE(new instruct::WAI())
  END_INSTRUCTION("WAI");

  proto->setMember("__get_register_size", TypePtr(new InternalFunctionValue("__get_register_size", LanguagePlugin::get_register_size)));
  proto->setMember("__set_register_size", TypePtr(new InternalFunctionValue("__set_register_size", LanguagePlugin::set_register_size)));
  proto->setMember(".dp", TypePtr(new InternalFunctionValue("dp", LanguagePlugin::number_dp)));
  proto->setMember(".long", TypePtr(new InternalFunctionValue("long", LanguagePlugin::number_long_address)));
  proto->setMember(".long_address", TypePtr(new InternalFunctionValue("long_address", LanguagePlugin::number_long_address)));
  proto->setMember(".indirect", TypePtr(new InternalFunctionValue("indirect", LanguagePlugin::number_indirect)));

  callWrapper = new CallWrapper(proto->getMember("()"));
  proto->setMember("()", TypePtr(new InternalFunctionValue("()", std::bind(&CallWrapper::call_datablock, callWrapper, _1, _2))));
  


  Interpreter interpreter(project);
  ContextPtr context(new ProjectContext(&interpreter));

  ClassPtr Register = RegisterClass::create(root, klass);
  root->setMember("A", RegisterClass::createInstance(context, "A")->get());
  root->setMember("X", RegisterClass::createInstance(context, "X")->get());
  root->setMember("Y", RegisterClass::createInstance(context, "Y")->get());
  root->setMember("S", RegisterClass::createInstance(context, "S")->get());
  root->setMember("dp", TypePtr(new InternalFunctionValue("dp", LanguagePlugin::dp)));
  root->setMember("long", TypePtr(new InternalFunctionValue("long", LanguagePlugin::long_address)));
  root->setMember("long_address", TypePtr(new InternalFunctionValue("long_address", LanguagePlugin::long_address)));
  root->setMember("indirect", TypePtr(new InternalFunctionValue("indirect", LanguagePlugin::indirect)));
  root->setMember("register", TypePtr(new InternalFunctionValue("register", LanguagePlugin::_register)));
  root->setMember("param", TypePtr(new InternalFunctionValue("param", LanguagePlugin::param)));
  root->setMember("returns", TypePtr(new InternalFunctionValue("returns", LanguagePlugin::returns, DECORATORCALL_OUTER)));

  compiler->setMember("brk", TypePtr(new InternalFunctionValue("brk", LanguagePlugin::compiler_break)));

  ClassPrototypePtr memoryVariableProto(memoryVariable->getPrototype());
  memoryVariableProto->setMember(".dp", TypePtr(new InternalFunctionValue("dp", LanguagePlugin::number_dp)));
  memoryVariableProto->setMember(".long", TypePtr(new InternalFunctionValue("long", LanguagePlugin::number_long_address)));
  memoryVariableProto->setMember(".long_address", TypePtr(new InternalFunctionValue("long_address", LanguagePlugin::number_long_address)));
  memoryVariableProto->setMember(".indirect", TypePtr(new InternalFunctionValue("indirect", LanguagePlugin::number_indirect)));

  ClassPrototypePtr numberProto(number->getPrototype());
  numberProto->setMember(".dp", TypePtr(new InternalFunctionValue("dp", LanguagePlugin::number_dp)));
  numberProto->setMember(".long", TypePtr(new InternalFunctionValue("long", LanguagePlugin::number_long_address)));
  numberProto->setMember(".long_address", TypePtr(new InternalFunctionValue("long_address", LanguagePlugin::number_long_address)));
  numberProto->setMember(".indirect", TypePtr(new InternalFunctionValue("indirect", LanguagePlugin::number_indirect)));

  ClassPrototypePtr typedNumberProto(typedNumber->getPrototype());
  typedNumberProto->setMember(".dp", TypePtr(new InternalFunctionValue("dp", LanguagePlugin::number_dp)));
  typedNumberProto->setMember(".long", TypePtr(new InternalFunctionValue("long", LanguagePlugin::number_long_address)));
  typedNumberProto->setMember(".long_address", TypePtr(new InternalFunctionValue("long_address", LanguagePlugin::number_long_address)));
  typedNumberProto->setMember(".indirect", TypePtr(new InternalFunctionValue("indirect", LanguagePlugin::number_indirect)));

  ClassPrototypePtr symbolRefProto(symbolRef->getPrototype());
  symbolRefProto->setMember(".dp", TypePtr(new InternalFunctionValue("dp", LanguagePlugin::number_dp)));
  symbolRefProto->setMember(".long", TypePtr(new InternalFunctionValue("long", LanguagePlugin::number_long_address)));
  symbolRefProto->setMember(".long_address", TypePtr(new InternalFunctionValue("long_address", LanguagePlugin::number_long_address)));
  symbolRefProto->setMember(".indirect", TypePtr(new InternalFunctionValue("indirect", LanguagePlugin::number_indirect)));
  symbolRefProto->setMember(".bank", TypePtr(new InternalFunctionValue("bank", LanguagePlugin::bank)));
  symbolRefProto->setMember(".bank_address", TypePtr(new InternalFunctionValue("bank_address", LanguagePlugin::bank_address)));

  ClassPrototypePtr functionProto(function->getPrototype());
  functionProto->setMember("!()", TypePtr(new InternalFunctionValue("!()", LanguagePlugin::function_jsr)));

  TypePtr container = root->getMember(FMA_TYPE_DECORATORCONTAINER_MEMBER);
  if (container->isDecoratorContainer()) {
    DecoratorContainerPtr containerPtr(std::dynamic_pointer_cast<DecoratorContainer>(container));
    GroupedParameterList params;
    DecoratorPtr decorator(new Decorator(TypePtr(new InternalFunctionValue("auto_insert_rts", LanguagePlugin::auto_insert_rts)), params));
    containerPtr->registerCallback(decorator);
  }

  return true;
}

// ----------------------------------------------------------------------------
void LanguagePlugin::release() {
  delete callWrapper;
}

// ----------------------------------------------------------------------------
uint8_t LanguagePlugin::getImmediateSize(const FMA::interpret::ContextPtr &context, const std::string &registerName) {
  const TypePtr &self = context->getInterpreter()->getGlobalContext()->getDeclareLevelObject();
  const TypePtr &value = self->getMember("__register_size_" + registerName);
  if(value->isUndefined()) {
    context->log().error() << "Unknown immediate size for register " << registerName;
    return 8;
  }

  return value->convertToNumber(context);
}

// ----------------------------------------------------------------------------
ResultPtr LanguagePlugin::_register(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeMap &args = parameter.only_kwargs();
  TypeMap::const_iterator it;

  const TypePtr &self = context->getInterpreter()->getGlobalContext()->getDeclareLevelObject();
  
  if ((it = args.find("A")) != args.end()) {
    self->setMember("__register_size_A", it->second);
  }

  if ((it = args.find("XY")) != args.end()) {
    self->setMember("__register_size_XY", it->second);
  }

  const TypeList &blocks = parameter.only_blocks();
  if (!blocks.size()) {
    return ResultPtr(new Result());
  }

  GroupedParameterList empty;
  return blocks.front()->call(context, empty);
}

// ----------------------------------------------------------------------------
void LanguagePlugin::transferRegisterType(const ContextPtr &context, const std::string &from, const std::string &to) {
  const ContextPtr &self = context->getRootLevelContext();

  TypePtr result = self->getMember(from)->get()->getMember("__register_type");
  self->getMember(to)->get()->setMember("__register_type", result);
}

// ----------------------------------------------------------------------------
ResultPtr LanguagePlugin::bank(const ContextPtr &context, const GroupedParameterList &) {
  GroupedParameterList params;
  params.push_back(core::NumberClass::createInstance(context, 16)->get());
  return context->self()->callDirect(">>", context, params);
}

// ----------------------------------------------------------------------------
ResultPtr LanguagePlugin::bank_address(const ContextPtr &context, const GroupedParameterList &) {
  GroupedParameterList params;
  params.push_back(core::NumberClass::createInstance(context, 0xFF0000)->get());
  return context->self()->callDirect("&", context, params);
}

// ----------------------------------------------------------------------------
ResultPtr LanguagePlugin::compiler_break(const ContextPtr &context, const GroupedParameterList &params) {
  ContextPtr global = context->getInterpreter()->getGlobalContext();
  if (!global) {
    context->log().error() << "Unable to access memory block in global context"; 
    return ResultPtr(new Result());
  }

  ResultPtr curBlock = global->resolve("::__current_block");
  if (!curBlock) {
    context->log().error() << "Unable to access memory block in global context"; 
    return ResultPtr(new Result());
  }

  plugin::MemoryBlock* memoryBlock = core::DataBlockClass::memoryBlock(context->getProject(), curBlock->get());
  if (!memoryBlock) {
    context->log().error() << "Unable to access memory block in global context"; 
    return ResultPtr(new Result());
  }

  const auto &kwArgs = params.only_kwargs();
  bool notifyOnly = false;
  std::string comment;
  TypeMap::const_iterator it;
  if ((it = kwArgs.find("notify_only")) != kwArgs.end()) {
    notifyOnly = it->second->convertToBoolean(context);
  }
  if ((it = kwArgs.find("comment")) != kwArgs.end()) {
    comment = it->second->convertToString(context);
  }

  auto *symbolMap = context->getProject()->getMemoryAdapter()->getSymbolMap();
  const auto &ref = symbolMap->createReference("..brk");
  memoryBlock->reference(ref);
  symbolMap->addEmulatorBreakpoint(ref, notifyOnly, comment);

  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
ResultPtr LanguagePlugin::returns(const ContextPtr &context, const GroupedParameterList &params) {
  const TypeList &blocks = params.only_blocks();
  if (!blocks.size()) {
    return ResultPtr(new Result());
  }

  const TypeMap &args = params.only_kwargs();
  TypeMap::const_iterator it;
  const ContextPtr &self = context->getRootLevelContext();
  if ((it = args.find("A")) != args.end()) {
    self->getMember("A")->get()->setMember("__register_type", it->second);
  }

  if ((it = args.find("X")) != args.end()) {
    self->getMember("X")->get()->setMember("__register_type", it->second);
  }

  if ((it = args.find("Y")) != args.end()) {
    self->getMember("Y")->get()->setMember("__register_type", it->second);
  }

  GroupedParameterList emptyParams;
  return blocks.front()->call(context, emptyParams);
}

// ----------------------------------------------------------------------------
ResultPtr LanguagePlugin::param(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeMap &args = parameter.only_kwargs();
  TypeMap::const_iterator it;

  const ContextPtr &self = context->getRootLevelContext();
  
  if ((it = args.find("A")) != args.end()) {
    self->getMember("A")->get()->setMember("__register_type", it->second);
  }

  if ((it = args.find("X")) != args.end()) {
    self->getMember("X")->get()->setMember("__register_type", it->second);
  }

  if ((it = args.find("Y")) != args.end()) {
    self->getMember("Y")->get()->setMember("__register_type", it->second);
  }

  const TypeList &blocks = parameter.only_blocks();
  if (!blocks.size()) {
    return ResultPtr(new Result());
  }

  GroupedParameterList empty;
  return blocks.front()->call(context, empty);
}

// ----------------------------------------------------------------------------
ResultPtr CallWrapper::call_datablock(const ContextPtr &context, const GroupedParameterList &parameter) {
  ContextPtr global = context->getInterpreter()->getGlobalContext();
  
  if (global) {
    ParameterList empty;
    ResultPtr curBlock = global->resolve("::__current_block");
    
    if (!curBlock->isUndefined()) {
      TypePtr block = curBlock->get();

      TypePtr A = block->getMember("__register_size_A");
      if (!A->isUndefined()) {
        context->self()->setMember("__register_size_A", A);
      }

      TypePtr XY = block->getMember("__register_size_XY");
      if (!XY->isUndefined()) {
        context->self()->setMember("__register_size_XY", XY);
      }
    }
  }

  return dataBlockCallOperator->call(context, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr LanguagePlugin::function_jsr(const ContextPtr &context, const GroupedParameterList &) {

  ContextPtr global = context->getInterpreter()->getGlobalContext();
  if (global) {
    ResultPtr jsr = global->resolve("::JSR");
    if (!jsr->isUndefined()) {
      GroupedParameterList params;
      params.push_back(context->self());

      jsr->pretendExecuted();
      jsr->get()->call(global, params);
      testTypeHinting(context);
    }
  }

  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
ResultPtr LanguagePlugin::auto_insert_rts(const ContextPtr &context, const GroupedParameterList &params) {
  GroupedParameterList empty;
  params.only_blocks().front()->call(context, empty);

  TypePtr type = context->getInterpreter()->getGlobalContext()->resolve("::__current_block")->get();
  plugin::MemoryBlock *block = core::DataBlockClass::memoryBlock(context);
  if (type->isObjectOfType("Function") && !block->isReturned()) {
    context->getInterpreter()->getGlobalContext()->resolve("::RTS")->get();
  }

  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
ResultPtr LanguagePlugin::number_indirect(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  ResultPtr result = core::TypedNumberClass::createInstance(context, context->self(), "indirect");

  if (args.size()) {
    result->get()->setMember("relative_to", args.front());
  }

  return result;
}

// ----------------------------------------------------------------------------
ResultPtr LanguagePlugin::indirect(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    return ResultPtr(new Result());
  }

  ContextPtr callContext(new InstanceContext(context->getInterpreter(), args.front()->asObject(), "indirect"));
  GroupedParameterList empty;
  if (args.size() == 2) {
    empty.push_back(args.back());
  }
  return args.front()->getDirectMember("indirect")->call(callContext, empty);
}

// ----------------------------------------------------------------------------
ResultPtr LanguagePlugin::number_dp(const ContextPtr &context, const GroupedParameterList &) {
  return core::TypedNumberClass::createInstance(context, context->self(), "dp");
}

// ----------------------------------------------------------------------------
ResultPtr LanguagePlugin::dp(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    return ResultPtr(new Result());
  }

  ContextPtr callContext(new InstanceContext(context->getInterpreter(), args.front()->asObject(), "dp"));
  GroupedParameterList empty;
  return args.front()->getDirectMember("dp")->call(callContext, empty);
}

// ----------------------------------------------------------------------------
ResultPtr LanguagePlugin::number_long_address(const ContextPtr &context, const GroupedParameterList &) {
  return core::TypedNumberClass::createInstance(context, context->self(), "long_address");
}

// ----------------------------------------------------------------------------
ResultPtr LanguagePlugin::long_address(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    return ResultPtr(new Result());
  }

  ContextPtr callContext(new InstanceContext(context->getInterpreter(), args.front()->asObject(), "long_address"));
  GroupedParameterList empty;
  return args.front()->getDirectMember("long_address")->call(callContext, empty);
}

// ----------------------------------------------------------------------------
ResultPtr LanguagePlugin::get_register_size(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (args.size() != 1) {
    return ResultPtr(new Result());
  }

  std::string registerName = args.front()->convertToString(context);

  if (registerName == "X" || registerName == "Y") {
    return ResultPtr(new Result(context, context->self()->getMember("__register_size_XY")));
  } else if (registerName == "A") {
    return ResultPtr(new Result(context, context->self()->getMember("__register_size_A")));
  }

  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
ResultPtr LanguagePlugin::set_register_size(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (args.size() != 2) {
    return ResultPtr(new Result());
  }

  std::string registerName = args.front()->convertToString(context);

  if (registerName == "X" || registerName == "Y") {
    context->self()->setMember("__register_size_XY", args.back());
  } else if (registerName == "A") {
    context->self()->setMember("__register_size_A", args.back());
  }

  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
void LanguagePlugin::setFlagRegister(const ContextPtr &context, uint8_t value) {
  if (value & 0x20) {
    context->self()->setMember("__register_size_A", core::NumberClass::createInstance(context, 8)->get());
  }
  if (value & 0x10) {
    context->self()->setMember("__register_size_XY", core::NumberClass::createInstance(context, 8)->get());
  }
}

// ----------------------------------------------------------------------------
void LanguagePlugin::resetFlagRegister(const ContextPtr &context, uint8_t value) {
  if (value & 0x20) {
    context->self()->setMember("__register_size_A", core::NumberClass::createInstance(context, 16)->get());
  }
  if (value & 0x10) {
    context->self()->setMember("__register_size_XY", core::NumberClass::createInstance(context, 16)->get());
  }
}

// ----------------------------------------------------------------------------
void LanguagePlugin::testTypeHinting(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter, const char *registerName) {
  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    return;
  }

  const TypePtr &value = args.front();
  TypePtr member = value->getMember("__typehint");
  if (!member->isUndefined()) {
    context->getRootLevelContext()->getMember(registerName)->get()->setMember("__register_type", member);
    return;
  }

}

// ----------------------------------------------------------------------------
void LanguagePlugin::testTypeHinting(const FMA::interpret::ContextPtr &context) {
  GroupedParameterList params;
  params.push_back(context->self());
  testTypeHinting(context, params);
}

// ----------------------------------------------------------------------------
void LanguagePlugin::testTypeHinting(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    return;
  }

  (void)context;
/*
  TypePtr value = args.front();
  std::cout << "TEST ON " << value->asString() << std::endl;
  if (value->isObjectOfType("Function")) {
    value->dump("");
    value = value->getMember("#macro");
  }
  std::cout << "#macro  " << value->asString() << std::endl;

  TypePtr member = value->getMember("__typehint_A");
  if (!member->isUndefined()) {
    context->getRootLevelContext()->getMember("A")->get()->setMember("__register_type", member);
    return;
  }

  member = value->getMember("__typehint_X");
  if (!member->isUndefined()) {
    context->getRootLevelContext()->getMember("X")->get()->setMember("__register_type", member);
    return;
  }

  member = value->getMember("__typehint_Y");
  if (!member->isUndefined()) {
    context->getRootLevelContext()->getMember("Y")->get()->setMember("__register_type", member);
    return;
  }*/
}

// ----------------------------------------------------------------------------