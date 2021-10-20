#include <superfx/SuperFxDataBlockClass.hpp>
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
using namespace FMA::superfx;
using namespace FMA::core;




// ----------------------------------------------------------------------------
ClassPtr SuperFxDataBlockClass::create(const ModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("SuperFxDataBlock", "SuperFx::SuperFxDataBlock"));
  klass->extends(ClassObject);

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", SuperFxDataBlockClass::initialize)));

  proto->setMember("LDA", TypePtr(new InternalFunctionValue("__invalid", SuperFxDataBlockClass::__invalid)));

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

