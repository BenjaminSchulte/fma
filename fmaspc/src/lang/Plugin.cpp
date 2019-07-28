#include <fma/assem/RegisterOperand.hpp>
#include <fma/assem/BitMaskOperand.hpp>
#include <fma/assem/LocalAddressOperand.hpp>
#include <fma/assem/PcRelativeAddressOperand.hpp>
#include <fma/assem/ConstantNumberOperand.hpp>
#include <fma/assem/FlagOperand.hpp>
#include <fma/Project.hpp>
#include <fma/core/DataBlock.hpp>
#include <fma/core/TypedNumber.hpp>

#include <lang/Plugin.hpp>
#include <lang/InstructionArguments.hpp>
#include <lang/Register.hpp>
#include <fma/plugin/MemoryPluginAdapter.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/types/Decorator.hpp>
#include <fma/types/DecoratorContainer.hpp>
#include <fma/interpret/Interpreter.hpp>
#include <fma/interpret/ProjectContext.hpp>
#include <fma/interpret/InstanceContext.hpp>

#include <fma/instruct/Adc.hpp>
#include <fma/instruct/Addw.hpp>
#include <fma/instruct/And.hpp>
#include <fma/instruct/Andnot.hpp>
#include <fma/instruct/Asl.hpp>
#include <fma/instruct/Brk.hpp>
#include <fma/instruct/Call.hpp>
#include <fma/instruct/Cmp.hpp>
#include <fma/instruct/Cmpjne.hpp>
#include <fma/instruct/Dec.hpp>
#include <fma/instruct/Decw.hpp>
#include <fma/instruct/Decjnz.hpp>
#include <fma/instruct/Eor.hpp>
#include <fma/instruct/Inc.hpp>
#include <fma/instruct/Incw.hpp>
#include <fma/instruct/Mul.hpp>
#include <fma/instruct/Not.hpp>
#include <fma/instruct/Div.hpp>
#include <fma/instruct/Jmp.hpp>
#include <fma/instruct/Jc.hpp>
#include <fma/instruct/Jnc.hpp>
#include <fma/instruct/Jbs.hpp>
#include <fma/instruct/Jbc.hpp>
#include <fma/instruct/Jz.hpp>
#include <fma/instruct/Jnz.hpp>
#include <fma/instruct/Js.hpp>
#include <fma/instruct/Jns.hpp>
#include <fma/instruct/Mov.hpp>
#include <fma/instruct/Movi.hpp>
#include <fma/instruct/Movw.hpp>
#include <fma/instruct/Or.hpp>
#include <fma/instruct/Lsr.hpp>
#include <fma/instruct/Nop.hpp>
#include <fma/instruct/Rol.hpp>
#include <fma/instruct/Ror.hpp>
#include <fma/instruct/Push.hpp>
#include <fma/instruct/Pop.hpp>
#include <fma/instruct/Rts.hpp>
#include <fma/instruct/Sbc.hpp>
#include <fma/instruct/Subw.hpp>
#include <fma/instruct/Xchg.hpp>

#include <iostream>

using namespace FMA;
using namespace FMA::assem;
using namespace FMA::types;
using namespace FMA::interpret;
using namespace FMASPC::lang;
using namespace std::placeholders;

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

#define RET_INSTRUCTION(name) \
  INSTRUCTION_START(name) \
  block->setLastIsReturn(true); \
  INSTRUCTION_START_VARIANTS()

#define CREATE(object) \
      Instruction *__inst = object; \
      block->write(__inst); \
      return ResultPtr(new Result());
      
#define IMPLICIT() \
    } else if (args.getLeft() == NULL && args.getRight() == NULL) {
      
#define VARIANT1(left) \
    } else if (args.getLeft() && args.getLeft()->left() && args.getRight() == NULL) {

#define VARIANT2(left,right) \
    } else if (args.getLeft() && args.getRight() && args.getLeft()->left() && args.getRight()->right()) {

#define VARIANT3(left,right,add) \
    } else if (args.getLeft() && args.getRight() && args.getAdd() && args.getLeft()->left() && args.getRight()->right() && args.getAdd()->add()) {

#define END_INSTRUCTION(name) \
    } \
    context->log().error() << "Invalid parameters for " << name << " instruction: " << args.asString(); \
    return ResultPtr(new Result()); \
  }))); \
  proto->setMember("::" name, proto->getMember(name))

// ----------------------------------------------------------------------------
SpcLanguagePlugin::SpcLanguagePlugin(Project *project)
  : FMA::plugin::LanguagePlugin(project)
{
}

// ----------------------------------------------------------------------------
bool SpcLanguagePlugin::initialize() {
  const RootModulePtr &root = project->root();
  const ClassPtr &klass = root->getMember("Class")->asClass();
  const ClassPtr &dataBlock = root->getMember("DataBlock")->asClass();
  const ClassPtr &number = root->getMember("Number")->asClass();
  const ClassPtr &typedNumber = root->getMember("TypedNumber")->asClass();
  const ClassPtr &memoryVariable = root->getMember("MemoryDeclaration")->asClass();
  const ClassPtr &function = root->getMember("Function")->asClass();
  const ClassPtr &symbolRef = root->getMember("SymbolReference")->asClass();
  if (!dataBlock || !klass || !number || !typedNumber || !memoryVariable || !symbolRef || !function) {
    project->log().error() << "Could not find DataBlock class";
    return false;
  }
  
  ClassPrototypePtr proto(dataBlock->getPrototype());
  INSTRUCTION("ADC");
    VARIANT2(regA, immediate) CREATE(new instruct::ADC(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, dp)        CREATE(new instruct::ADC(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, address)   CREATE(new instruct::ADC(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, addressX)  CREATE(new instruct::ADC(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, addressY)  CREATE(new instruct::ADC(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, indirectY) CREATE(new instruct::ADC(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, indirectX) CREATE(new instruct::ADC(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, dataFromX) CREATE(new instruct::ADC(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(dp, dp)          CREATE(new instruct::ADC(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(dp, immediate)   CREATE(new instruct::ADC(args.createLeftOperand(), args.createRightOperand()))
  END_INSTRUCTION("ADC");
  INSTRUCTION("ADDW");
    VARIANT2(regYA, dp)       CREATE(new instruct::ADDW(args.createLeftOperand(), args.createRightOperand()))
  END_INSTRUCTION("ADDW");
  INSTRUCTION("AND");
    VARIANT2(regA, immediate) CREATE(new instruct::AND(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, dp)        CREATE(new instruct::AND(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, address)   CREATE(new instruct::AND(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, addressX)  CREATE(new instruct::AND(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, addressY)  CREATE(new instruct::AND(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, indirectY) CREATE(new instruct::AND(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, indirectX) CREATE(new instruct::AND(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, dataFromX) CREATE(new instruct::AND(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(dp, dp)          CREATE(new instruct::AND(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(dp, immediate)   CREATE(new instruct::AND(args.createLeftOperand(), args.createRightOperand()))
  END_INSTRUCTION("AND");
  INSTRUCTION("ASL");
    IMPLICIT()                CREATE(new instruct::ASL(new RegisterOperand("A"), new ConstantNumberOperand(1)))
    VARIANT1(regA)            CREATE(new instruct::ASL(args.createOperand(), new ConstantNumberOperand(1)))
    VARIANT1(dp)              CREATE(new instruct::ASL(args.createOperand(), new ConstantNumberOperand(1)))
    VARIANT1(address)         CREATE(new instruct::ASL(args.createOperand(), new ConstantNumberOperand(1)))
  END_INSTRUCTION("ASL");
  INSTRUCTION("BRK");
    IMPLICIT()                CREATE(new instruct::BRK())
  END_INSTRUCTION("BRK");
  INSTRUCTION("BBS0");
    VARIANT2(dp, address)     CREATE(new instruct::JBS(args.createLeftOperand(), args.createRightOperand(), new ConstantNumberOperand(1 << 0)))
  END_INSTRUCTION("BBS0");
  INSTRUCTION("BBS1");
    VARIANT2(dp, address)     CREATE(new instruct::JBS(args.createLeftOperand(), args.createRightOperand(), new ConstantNumberOperand(1 << 1)))
  END_INSTRUCTION("BBS1");
  INSTRUCTION("BBS2");
    VARIANT2(dp, address)     CREATE(new instruct::JBS(args.createLeftOperand(), args.createRightOperand(), new ConstantNumberOperand(1 << 2)))
  END_INSTRUCTION("BBS2");
  INSTRUCTION("BBS3");
    VARIANT2(dp, address)     CREATE(new instruct::JBS(args.createLeftOperand(), args.createRightOperand(), new ConstantNumberOperand(1 << 3)))
  END_INSTRUCTION("BBS3");
  INSTRUCTION("BBS4");
    VARIANT2(dp, address)     CREATE(new instruct::JBS(args.createLeftOperand(), args.createRightOperand(), new ConstantNumberOperand(1 << 4)))
  END_INSTRUCTION("BBS4");
  INSTRUCTION("BBS5");
    VARIANT2(dp, address)     CREATE(new instruct::JBS(args.createLeftOperand(), args.createRightOperand(), new ConstantNumberOperand(1 << 5)))
  END_INSTRUCTION("BBS5");
  INSTRUCTION("BBS6");
    VARIANT2(dp, address)     CREATE(new instruct::JBS(args.createLeftOperand(), args.createRightOperand(), new ConstantNumberOperand(1 << 6)))
  END_INSTRUCTION("BBS6");
  INSTRUCTION("BBS7");
    VARIANT2(dp, address)     CREATE(new instruct::JBS(args.createLeftOperand(), args.createRightOperand(), new ConstantNumberOperand(1 << 7)))
  END_INSTRUCTION("BBS7");
  INSTRUCTION("BBC0");
    VARIANT2(dp, address)     CREATE(new instruct::JBC(args.createLeftOperand(), args.createRightOperand(), new ConstantNumberOperand(1 << 0)))
  END_INSTRUCTION("BBC0");
  INSTRUCTION("BBC1");
    VARIANT2(dp, address)     CREATE(new instruct::JBC(args.createLeftOperand(), args.createRightOperand(), new ConstantNumberOperand(1 << 1)))
  END_INSTRUCTION("BBC1");
  INSTRUCTION("BBC2");
    VARIANT2(dp, address)     CREATE(new instruct::JBC(args.createLeftOperand(), args.createRightOperand(), new ConstantNumberOperand(1 << 2)))
  END_INSTRUCTION("BBC2");
  INSTRUCTION("BBC3");
    VARIANT2(dp, address)     CREATE(new instruct::JBC(args.createLeftOperand(), args.createRightOperand(), new ConstantNumberOperand(1 << 3)))
  END_INSTRUCTION("BBC3");
  INSTRUCTION("BBC4");
    VARIANT2(dp, address)     CREATE(new instruct::JBC(args.createLeftOperand(), args.createRightOperand(), new ConstantNumberOperand(1 << 4)))
  END_INSTRUCTION("BBC4");
  INSTRUCTION("BBC5");
    VARIANT2(dp, address)     CREATE(new instruct::JBC(args.createLeftOperand(), args.createRightOperand(), new ConstantNumberOperand(1 << 5)))
  END_INSTRUCTION("BBC5");
  INSTRUCTION("BBC6");
    VARIANT2(dp, address)     CREATE(new instruct::JBC(args.createLeftOperand(), args.createRightOperand(), new ConstantNumberOperand(1 << 6)))
  END_INSTRUCTION("BBC6");
  INSTRUCTION("BBC7");
    VARIANT2(dp, address)     CREATE(new instruct::JBC(args.createLeftOperand(), args.createRightOperand(), new ConstantNumberOperand(1 << 7)))
  END_INSTRUCTION("BBC7");
  INSTRUCTION("BCC");
    VARIANT1(address)         CREATE(new instruct::JNC(new PcRelativeAddressOperand(args.getLeft()->createValueOperand())));
  END_INSTRUCTION("BCC");
  INSTRUCTION("BLT");
    VARIANT1(address)         CREATE(new instruct::JNC(new PcRelativeAddressOperand(args.getLeft()->createValueOperand())));
  END_INSTRUCTION("BLT");
  INSTRUCTION("BGE");
    VARIANT1(address)         CREATE(new instruct::JC(new PcRelativeAddressOperand(args.getLeft()->createValueOperand())));
  END_INSTRUCTION("BGE");
  INSTRUCTION("BCS");
    VARIANT1(address)         CREATE(new instruct::JC(new PcRelativeAddressOperand(args.getLeft()->createValueOperand())));
  END_INSTRUCTION("BCS");
  INSTRUCTION("BEQ");
    VARIANT1(address)         CREATE(new instruct::JZ(new PcRelativeAddressOperand(args.getLeft()->createValueOperand())));
  END_INSTRUCTION("BEQ");
  INSTRUCTION("BZE");
    VARIANT1(address)         CREATE(new instruct::JZ(new PcRelativeAddressOperand(args.getLeft()->createValueOperand())));
  END_INSTRUCTION("BZE");
  INSTRUCTION("BNE");
    VARIANT1(address)         CREATE(new instruct::JNZ(new PcRelativeAddressOperand(args.getLeft()->createValueOperand())));
  END_INSTRUCTION("BNE");
  INSTRUCTION("BNZ");
    VARIANT1(address)         CREATE(new instruct::JNZ(new PcRelativeAddressOperand(args.getLeft()->createValueOperand())));
  END_INSTRUCTION("BNZ");
  INSTRUCTION("BMI");
    VARIANT1(address)         CREATE(new instruct::JS(new PcRelativeAddressOperand(args.getLeft()->createValueOperand())));
  END_INSTRUCTION("BMI");
  INSTRUCTION("BPL");
    VARIANT1(address)         CREATE(new instruct::JNS(new PcRelativeAddressOperand(args.getLeft()->createValueOperand())));
  END_INSTRUCTION("BPL");
  RET_INSTRUCTION("BRA");
    VARIANT1(address)         CREATE(new instruct::JMP(new PcRelativeAddressOperand(args.getLeft()->createValueOperand())));
  END_INSTRUCTION("BRA");
  INSTRUCTION("CLR0");
    VARIANT1(dp)              CREATE(new instruct::ANDNOT(args.createLeftOperand(), new ConstantNumberOperand(1 << 0)))
  END_INSTRUCTION("CLR0");
  INSTRUCTION("CLR1");
    VARIANT1(dp)              CREATE(new instruct::ANDNOT(args.createLeftOperand(), new ConstantNumberOperand(1 << 1)))
  END_INSTRUCTION("CLR1");
  INSTRUCTION("CLR2");
    VARIANT1(dp)              CREATE(new instruct::ANDNOT(args.createLeftOperand(), new ConstantNumberOperand(1 << 2)))
  END_INSTRUCTION("CLR2");
  INSTRUCTION("CLR3");
    VARIANT1(dp)              CREATE(new instruct::ANDNOT(args.createLeftOperand(), new ConstantNumberOperand(1 << 3)))
  END_INSTRUCTION("CLR3");
  INSTRUCTION("CLR4");
    VARIANT1(dp)              CREATE(new instruct::ANDNOT(args.createLeftOperand(), new ConstantNumberOperand(1 << 4)))
  END_INSTRUCTION("CLR4");
  INSTRUCTION("CLR5");
    VARIANT1(dp)              CREATE(new instruct::ANDNOT(args.createLeftOperand(), new ConstantNumberOperand(1 << 5)))
  END_INSTRUCTION("CLR5");
  INSTRUCTION("CLR6");
    VARIANT1(dp)              CREATE(new instruct::ANDNOT(args.createLeftOperand(), new ConstantNumberOperand(1 << 6)))
  END_INSTRUCTION("CLR6");
  INSTRUCTION("CLR7");
    VARIANT1(dp)              CREATE(new instruct::ANDNOT(args.createLeftOperand(), new ConstantNumberOperand(1 << 7)))
  END_INSTRUCTION("CLR7");
  INSTRUCTION("CLRC");
    IMPLICIT()                CREATE(new instruct::ANDNOT(new RegisterOperand("P"), new ConstantNumberOperand(0x01)))
  END_INSTRUCTION("CLRC");
  INSTRUCTION("CLRP");
    IMPLICIT()                CREATE(new instruct::ANDNOT(new RegisterOperand("P"), new ConstantNumberOperand(0x20)))
  END_INSTRUCTION("CLRP");
  INSTRUCTION("CMP");
    VARIANT2(regA, address)   CREATE(new instruct::CMP(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, addressX)  CREATE(new instruct::CMP(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, addressY)  CREATE(new instruct::CMP(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, dp)        CREATE(new instruct::CMP(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, immediate) CREATE(new instruct::CMP(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regX, immediate) CREATE(new instruct::CMP(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regX, dp)        CREATE(new instruct::CMP(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regY, immediate) CREATE(new instruct::CMP(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regY, dp)        CREATE(new instruct::CMP(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(dp, dp)          CREATE(new instruct::CMP(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(dp, immediate)   CREATE(new instruct::CMP(args.createLeftOperand(), args.createRightOperand()))
  END_INSTRUCTION("CMP");
  INSTRUCTION("CBNE");
    VARIANT2(dp, address)     CREATE(new instruct::CMPJNE(new RegisterOperand("A"), args.createLeftOperand(), args.createRightOperand()))
  END_INSTRUCTION("CMP");
  INSTRUCTION("DBNZ");
    VARIANT2(dp, address)     CREATE(new instruct::DECJNZ(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regY, address)   CREATE(new instruct::DECJNZ(new RegisterOperand("Y"), args.createRightOperand()))
  END_INSTRUCTION("DBNZ");
  INSTRUCTION("DEC");
    IMPLICIT()                CREATE(new instruct::DEC(new RegisterOperand("A")))
    VARIANT1(address)         CREATE(new instruct::DEC(args.createOperand()))
    VARIANT1(dpX)             CREATE(new instruct::DEC(args.createOperand()))
    VARIANT1(dp)              CREATE(new instruct::DEC(args.createOperand()))
    VARIANT1(regA)            CREATE(new instruct::DEC(args.createOperand()))
    VARIANT1(regX)            CREATE(new instruct::DEC(args.createOperand()))
    VARIANT1(regY)            CREATE(new instruct::DEC(args.createOperand()))
  END_INSTRUCTION("DEC");
  INSTRUCTION("DECW");
    VARIANT1(dp)              CREATE(new instruct::DECW(args.createOperand()))
  END_INSTRUCTION("DECW");
  INSTRUCTION("DIV");
    IMPLICIT()                CREATE(new instruct::DIV(new RegisterOperand("YA"), new RegisterOperand("X")))
    VARIANT2(regYA, regX)     CREATE(new instruct::DIV(new RegisterOperand("YA"), new RegisterOperand("X")))
  END_INSTRUCTION("DIV");
  INSTRUCTION("EOR");
    VARIANT2(regA, immediate) CREATE(new instruct::EOR(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, dp)        CREATE(new instruct::EOR(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, address)   CREATE(new instruct::EOR(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, addressX)  CREATE(new instruct::EOR(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, addressY)  CREATE(new instruct::EOR(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, indirectX) CREATE(new instruct::EOR(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, indirectY) CREATE(new instruct::EOR(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, dataFromX) CREATE(new instruct::EOR(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(dp, dp)          CREATE(new instruct::EOR(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(dp, immediate)   CREATE(new instruct::EOR(args.createLeftOperand(), args.createRightOperand()))
  END_INSTRUCTION("EOR");
  INSTRUCTION("INC");
    IMPLICIT()                CREATE(new instruct::INC(new RegisterOperand("A")))
    VARIANT1(dp)              CREATE(new instruct::INC(args.createOperand()))
    VARIANT1(address)         CREATE(new instruct::INC(args.createOperand()))
    VARIANT1(regA)            CREATE(new instruct::INC(args.createOperand()))
    VARIANT1(regX)            CREATE(new instruct::INC(args.createOperand()))
    VARIANT1(regY)            CREATE(new instruct::INC(args.createOperand()))
  END_INSTRUCTION("INC");
  INSTRUCTION("INCW");
    VARIANT1(dp)              CREATE(new instruct::INCW(args.createOperand()))
  END_INSTRUCTION("INCW");
  INSTRUCTION("CALL");
    VARIANT1(address)         CREATE(new instruct::CALL(new LocalAddressOperand(args.getLeft()->createValueOperand())));
  END_INSTRUCTION("CALL");
  RET_INSTRUCTION("JMP");
    VARIANT1(address)         CREATE(new instruct::JMP(args.getLeft()->createValueOperand()));
    VARIANT1(addressX)        CREATE(new instruct::JMP(args.createLeftOperand()));
  END_INSTRUCTION("JMP");
  INSTRUCTION("LSR");
    IMPLICIT()                CREATE(new instruct::LSR(new RegisterOperand("A"), new ConstantNumberOperand(1)))
    VARIANT1(regA)            CREATE(new instruct::LSR(args.createOperand(), new ConstantNumberOperand(1)))
    VARIANT1(dp)              CREATE(new instruct::LSR(args.createOperand(), new ConstantNumberOperand(1)))
    VARIANT1(address)         CREATE(new instruct::LSR(args.createOperand(), new ConstantNumberOperand(1)))
  END_INSTRUCTION("LSR");
  INSTRUCTION("MUL");
    IMPLICIT()                CREATE(new instruct::MUL(new RegisterOperand("Y"), new RegisterOperand("A")))
    VARIANT1(regYA)           CREATE(new instruct::MUL(new RegisterOperand("Y"), new RegisterOperand("A")))
  END_INSTRUCTION("MUL");
  INSTRUCTION("MOV");
    VARIANT2(address, regA)   CREATE(new instruct::MOV(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(address, regX)   CREATE(new instruct::MOV(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(address, regY)   CREATE(new instruct::MOV(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(addressX, regA)  CREATE(new instruct::MOV(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(addressY, regA)  CREATE(new instruct::MOV(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, address)   CREATE(new instruct::MOV(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, addressX)  CREATE(new instruct::MOV(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, addressY)  CREATE(new instruct::MOV(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, dp)        CREATE(new instruct::MOV(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, immediate) CREATE(new instruct::MOV(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, indirect)  CREATE(new instruct::MOV(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, indirectY) CREATE(new instruct::MOV(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, dataFromX) CREATE(new instruct::MOV(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, indirectX) CREATE(new instruct::MOV(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, regX)      CREATE(new instruct::MOV(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, regY)      CREATE(new instruct::MOV(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regX, immediate) CREATE(new instruct::MOV(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regX, regA)      CREATE(new instruct::MOV(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regX, dp)        CREATE(new instruct::MOV(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regY, immediate) CREATE(new instruct::MOV(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regY, regA)      CREATE(new instruct::MOV(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regY, dp)        CREATE(new instruct::MOV(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(dp, dp)          CREATE(new instruct::MOV(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(dp, immediate)   CREATE(new instruct::MOV(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(dp, regA)        CREATE(new instruct::MOV(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(dp, regX)        CREATE(new instruct::MOV(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(dp, regY)        CREATE(new instruct::MOV(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(dpX, regA)       CREATE(new instruct::MOV(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(indirectY, regA) CREATE(new instruct::MOV(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(dataFromX, regA) CREATE(new instruct::MOV(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regS, regX)      CREATE(new instruct::MOV(args.createLeftOperand(), args.createRightOperand()))
  END_INSTRUCTION("MOV");
  INSTRUCTION("MOV1");
    VARIANT3(flagC, address, address) CREATE(new instruct::MOV(new FlagOperand("C"), new BitMaskOperand(args.createRightOperand(), new ConstantNumberOperand(0), args.createThirdOperand(), 1)))
    VARIANT3(address, address, flagC) CREATE(new instruct::MOV(new BitMaskOperand(args.createLeftOperand(), new ConstantNumberOperand(0), args.createRightOperand(), 1), new FlagOperand("C")))
  END_INSTRUCTION("MOV1");
  INSTRUCTION("MOVI");
    VARIANT2(regA, dataFromX) CREATE(new instruct::MOVI(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(dataFromX, regA) CREATE(new instruct::MOVI(args.createLeftOperand(), args.createRightOperand()))
  END_INSTRUCTION("MOVI");
  INSTRUCTION("MOVW");
    VARIANT2(dp, regYA)       CREATE(new instruct::MOVW(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regYA, dp)       CREATE(new instruct::MOVW(args.createLeftOperand(), args.createRightOperand()))
  END_INSTRUCTION("MOVW");
  INSTRUCTION("NOT1");
    VARIANT2(address, address) CREATE(new instruct::NOT(new BitMaskOperand(args.createLeftOperand(), new ConstantNumberOperand(0), args.createRightOperand(), 1), new ConstantNumberOperand(0)))
  END_INSTRUCTION("NOT1");
  INSTRUCTION("NOP");
    IMPLICIT()                CREATE(new instruct::NOP())
  END_INSTRUCTION("NOP");
  INSTRUCTION("OR");
    VARIANT2(regA, immediate) CREATE(new instruct::OR(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, dp)        CREATE(new instruct::OR(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, address)   CREATE(new instruct::OR(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, addressX)  CREATE(new instruct::OR(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, addressY)  CREATE(new instruct::OR(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, indirectX) CREATE(new instruct::OR(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, indirectY) CREATE(new instruct::OR(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, dataFromX) CREATE(new instruct::OR(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(dp, dp)          CREATE(new instruct::OR(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(dp, immediate)   CREATE(new instruct::OR(args.createLeftOperand(), args.createRightOperand()))
  END_INSTRUCTION("OR");
  RET_INSTRUCTION("RET");
    IMPLICIT()                CREATE(new instruct::RTS())
  END_INSTRUCTION("RET");
  INSTRUCTION("ROL");
    IMPLICIT()                CREATE(new instruct::ROL(new RegisterOperand("A"), new ConstantNumberOperand(1)))
    VARIANT1(regA)            CREATE(new instruct::ROL(args.createOperand(), new ConstantNumberOperand(1)))
    VARIANT1(dp)              CREATE(new instruct::ROL(args.createOperand(), new ConstantNumberOperand(1)))
    VARIANT1(address)         CREATE(new instruct::ROL(args.createOperand(), new ConstantNumberOperand(1)))
  END_INSTRUCTION("ROL");
  INSTRUCTION("ROR");
    IMPLICIT()                CREATE(new instruct::ROR(new RegisterOperand("A"), new ConstantNumberOperand(1)))
    VARIANT1(regA)            CREATE(new instruct::ROR(args.createOperand(), new ConstantNumberOperand(1)))
    VARIANT1(dp)              CREATE(new instruct::ROR(args.createOperand(), new ConstantNumberOperand(1)))
    VARIANT1(address)         CREATE(new instruct::ROR(args.createOperand(), new ConstantNumberOperand(1)))
  END_INSTRUCTION("ROR");
  INSTRUCTION("SBC");
    VARIANT2(regA, immediate) CREATE(new instruct::SBC(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, dp)        CREATE(new instruct::SBC(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, address)   CREATE(new instruct::SBC(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, addressX)  CREATE(new instruct::SBC(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, addressY)  CREATE(new instruct::SBC(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, indirectX) CREATE(new instruct::SBC(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, indirectY) CREATE(new instruct::SBC(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(regA, dataFromX) CREATE(new instruct::SBC(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(dp, dp)          CREATE(new instruct::SBC(args.createLeftOperand(), args.createRightOperand()))
    VARIANT2(dp, immediate)   CREATE(new instruct::SBC(args.createLeftOperand(), args.createRightOperand()))
  END_INSTRUCTION("SBC");
  INSTRUCTION("PUSH");
    VARIANT1(regA)            CREATE(new instruct::PUSH(new RegisterOperand("A")));
    VARIANT1(regX)            CREATE(new instruct::PUSH(new RegisterOperand("X")));
    VARIANT1(regY)            CREATE(new instruct::PUSH(new RegisterOperand("Y")));
  END_INSTRUCTION("PUSH");
  INSTRUCTION("POP");
    VARIANT1(regA)            CREATE(new instruct::POP(new RegisterOperand("A")));
    VARIANT1(regX)            CREATE(new instruct::POP(new RegisterOperand("X")));
    VARIANT1(regY)            CREATE(new instruct::POP(new RegisterOperand("Y")));
  END_INSTRUCTION("POP");
  INSTRUCTION("SET0");
    VARIANT1(dp)              CREATE(new instruct::OR(args.createLeftOperand(), new ConstantNumberOperand(1 << 0)))
  END_INSTRUCTION("SET0");
  INSTRUCTION("SET1");
    VARIANT1(dp)              CREATE(new instruct::OR(args.createLeftOperand(), new ConstantNumberOperand(1 << 1)))
  END_INSTRUCTION("SET1");
  INSTRUCTION("SET2");
    VARIANT1(dp)              CREATE(new instruct::OR(args.createLeftOperand(), new ConstantNumberOperand(1 << 2)))
  END_INSTRUCTION("SET2");
  INSTRUCTION("SET3");
    VARIANT1(dp)              CREATE(new instruct::OR(args.createLeftOperand(), new ConstantNumberOperand(1 << 3)))
  END_INSTRUCTION("SET3");
  INSTRUCTION("SET4");
    VARIANT1(dp)              CREATE(new instruct::OR(args.createLeftOperand(), new ConstantNumberOperand(1 << 4)))
  END_INSTRUCTION("SET4");
  INSTRUCTION("SET5");
    VARIANT1(dp)              CREATE(new instruct::OR(args.createLeftOperand(), new ConstantNumberOperand(1 << 5)))
  END_INSTRUCTION("SET5");
  INSTRUCTION("SET6");
    VARIANT1(dp)              CREATE(new instruct::OR(args.createLeftOperand(), new ConstantNumberOperand(1 << 6)))
  END_INSTRUCTION("SET6");
  INSTRUCTION("SET7");
    VARIANT1(dp)              CREATE(new instruct::OR(args.createLeftOperand(), new ConstantNumberOperand(1 << 7)))
  END_INSTRUCTION("SET7");
  INSTRUCTION("SETC");
    IMPLICIT()                CREATE(new instruct::OR(new RegisterOperand("P"), new ConstantNumberOperand(0x01)))
  END_INSTRUCTION("SETC");
  INSTRUCTION("SETP");
    IMPLICIT()                CREATE(new instruct::OR(new RegisterOperand("P"), new ConstantNumberOperand(0x20)))
  END_INSTRUCTION("SETP");
  INSTRUCTION("SUBW");
    VARIANT2(regYA, dp)       CREATE(new instruct::SUBW(args.createLeftOperand(), args.createRightOperand()))
  END_INSTRUCTION("SUBW");
  INSTRUCTION("XCN");
    IMPLICIT()                CREATE(new instruct::XCHG(new BitMaskOperand(new RegisterOperand("A"), 0, 4, 0xF), new BitMaskOperand(new RegisterOperand("A"), 4, 0, 0xF)))
    VARIANT1(regA)            CREATE(new instruct::XCHG(new BitMaskOperand(new RegisterOperand("A"), 0, 4, 0xF), new BitMaskOperand(new RegisterOperand("A"), 4, 0, 0xF)))
  END_INSTRUCTION("XCN");

  proto->setMember(".dp", TypePtr(new InternalFunctionValue(".dp", SpcLanguagePlugin::number_dp)));
  proto->setMember(".indirect", TypePtr(new InternalFunctionValue(".indirect", SpcLanguagePlugin::number_indirect)));
  proto->setMember(".addr", TypePtr(new InternalFunctionValue(".addr", SpcLanguagePlugin::number_addr)));

  Interpreter interpreter(project);
  ContextPtr context(new ProjectContext(&interpreter));
  ClassPtr Register = RegisterClass::create(root, klass);
  root->setMember("A", RegisterClass::createInstance(context, "A")->get());
  root->setMember("X", RegisterClass::createInstance(context, "X")->get());
  root->setMember("C", RegisterClass::createInstance(context, "C")->get());
  root->setMember("Y", RegisterClass::createInstance(context, "Y")->get());
  root->setMember("YA", RegisterClass::createInstance(context, "YA")->get());
  root->setMember("SP", RegisterClass::createInstance(context, "SP")->get());
  root->setMember("dp", TypePtr(new InternalFunctionValue("dp", SpcLanguagePlugin::dp)));
  root->setMember("addr", TypePtr(new InternalFunctionValue("addr", SpcLanguagePlugin::addr)));
  root->setMember("indirect", TypePtr(new InternalFunctionValue("indirect", SpcLanguagePlugin::indirect)));

  ClassPrototypePtr memoryVariableProto(memoryVariable->getPrototype());
  memoryVariableProto->setMember("dp", TypePtr(new InternalFunctionValue("dp", SpcLanguagePlugin::number_dp)));
  memoryVariableProto->setMember("indirect", TypePtr(new InternalFunctionValue("indirect", SpcLanguagePlugin::number_indirect)));
  memoryVariableProto->setMember("addr", TypePtr(new InternalFunctionValue("addr", SpcLanguagePlugin::number_addr)));

  ClassPrototypePtr numberProto(number->getPrototype());
  numberProto->setMember("dp", TypePtr(new InternalFunctionValue("dp", SpcLanguagePlugin::number_dp)));
  numberProto->setMember("indirect", TypePtr(new InternalFunctionValue("indirect", SpcLanguagePlugin::number_indirect)));
  numberProto->setMember("addr", TypePtr(new InternalFunctionValue("addr", SpcLanguagePlugin::number_addr)));

  ClassPrototypePtr registerProto(Register->getPrototype());
  registerProto->setMember("dp", TypePtr(new InternalFunctionValue("dp", SpcLanguagePlugin::number_dp)));
  registerProto->setMember("indirect", TypePtr(new InternalFunctionValue("indirect", SpcLanguagePlugin::number_indirect)));
  registerProto->setMember("addr", TypePtr(new InternalFunctionValue("addr", SpcLanguagePlugin::number_addr)));

  ClassPrototypePtr typedNumberProto(typedNumber->getPrototype());
  typedNumberProto->setMember("dp", TypePtr(new InternalFunctionValue("dp", SpcLanguagePlugin::number_dp)));
  typedNumberProto->setMember("indirect", TypePtr(new InternalFunctionValue("indirect", SpcLanguagePlugin::number_indirect)));
  typedNumberProto->setMember("addr", TypePtr(new InternalFunctionValue("addr", SpcLanguagePlugin::number_addr)));

  ClassPrototypePtr symbolRefProto(symbolRef->getPrototype());
  symbolRefProto->setMember("dp", TypePtr(new InternalFunctionValue("dp", SpcLanguagePlugin::number_dp)));
  symbolRefProto->setMember("indirect", TypePtr(new InternalFunctionValue("indirect", SpcLanguagePlugin::number_indirect)));
  symbolRefProto->setMember("addr", TypePtr(new InternalFunctionValue("addr", SpcLanguagePlugin::number_addr)));

  ClassPrototypePtr functionProto(function->getPrototype());
  functionProto->setMember("!()", TypePtr(new InternalFunctionValue("!()", SpcLanguagePlugin::function_jsr)));

  TypePtr container = root->getMember(FMA_TYPE_DECORATORCONTAINER_MEMBER);
  if (container->isDecoratorContainer()) {
    DecoratorContainerPtr containerPtr(std::dynamic_pointer_cast<DecoratorContainer>(container));
    GroupedParameterList params;
    DecoratorPtr decorator(new Decorator(TypePtr(new InternalFunctionValue("auto_insert_rts", SpcLanguagePlugin::auto_insert_rts)), params));
    containerPtr->registerCallback(decorator);
  }

  return true;
}

// ----------------------------------------------------------------------------
ResultPtr SpcLanguagePlugin::number_dp(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  ResultPtr result = core::TypedNumberClass::createInstance(context, context->self(), "dp");

  if (args.size()) {
    result->get()->setMember("relative_to", args.front());
  }

  return result;
}

// ----------------------------------------------------------------------------
ResultPtr SpcLanguagePlugin::number_indirect(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  ResultPtr result = core::TypedNumberClass::createInstance(context, context->self(), "indirect");

  if (args.size()) {
    result->get()->setMember("relative_to", args.front());
  }

  return result;
}

// ----------------------------------------------------------------------------
ResultPtr SpcLanguagePlugin::number_addr(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  ResultPtr result = core::TypedNumberClass::createInstance(context, context->self(), "addr");

  if (args.size()) {
    result->get()->setMember("relative_to", args.front());
  }

  return result;
}

// ----------------------------------------------------------------------------
ResultPtr SpcLanguagePlugin::addr(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    return ResultPtr(new Result());
  }

  ContextPtr callContext(new InstanceContext(context->getInterpreter(), args.front()->asObject(), "addr"));
  GroupedParameterList empty;
  if (args.size() == 2) {
    empty.push_back(args.back());
  }
  return args.front()->getDirectMember("addr")->call(callContext, empty);
}


// ----------------------------------------------------------------------------
ResultPtr SpcLanguagePlugin::indirect(const ContextPtr &context, const GroupedParameterList &parameter) {
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
ResultPtr SpcLanguagePlugin::dp(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    return ResultPtr(new Result());
  }

  ContextPtr callContext(new InstanceContext(context->getInterpreter(), args.front()->asObject(), "dp"));
  GroupedParameterList empty;
  if (args.size() == 2) {
    empty.push_back(args.back());
  }

  return args.front()->getDirectMember("dp")->call(callContext, empty);
}

// ----------------------------------------------------------------------------
ResultPtr SpcLanguagePlugin::function_jsr(const ContextPtr &context, const GroupedParameterList &) {
  ContextPtr global = context->getInterpreter()->getGlobalContext();
  if (global) {
    ResultPtr jsr = global->resolve("::CALL");
    if (!jsr->isUndefined()) {
      GroupedParameterList params;
      params.push_back(context->self());

      jsr->pretendExecuted();
      jsr->get()->call(global, params);
    }
  }

  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
ResultPtr SpcLanguagePlugin::auto_insert_rts(const ContextPtr &context, const GroupedParameterList &params) {
  GroupedParameterList empty;
  params.only_blocks().front()->call(context, empty);

  TypePtr type = context->getInterpreter()->getGlobalContext()->resolve("::__current_block")->get();
  plugin::MemoryBlock *block = core::DataBlockClass::memoryBlock(context);
  if (type->isObjectOfType("Function") && !block->isReturned()) {
    context->getInterpreter()->getGlobalContext()->resolve("::RET")->get();
  }

  return ResultPtr(new Result());
}


// ----------------------------------------------------------------------------
void SpcLanguagePlugin::release() {
}
