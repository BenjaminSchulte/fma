#include <superfx/Plugin.hpp>
#include <superfx/SuperFxDataBlockClass.hpp>
#include <fma/Project.hpp>
#include <fma/core/DataBlock.hpp>
#include <fma/plugin/MemoryPluginAdapter.hpp>
#include <fma/types/Module.hpp>
#include <fma/types/Class.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/InstanceContext.hpp>

#include <iostream>

using namespace FMA;
using namespace FMA::core;
using namespace FMA::superfx;
using namespace FMA::types;
using namespace FMA::interpret;

// ----------------------------------------------------------------------------
SuperFxPlugin::SuperFxPlugin(Project *project)
  : plugin::LanguagePlugin(project)
{
}

// ----------------------------------------------------------------------------
bool SuperFxPlugin::initialize() {
  const RootModulePtr &root = project->root();
  const ClassPtr &klass = root->getMember("Class")->asClass();
  const ClassPtr &DataBlock = root->getMember("DataBlock")->asClass();
  if (!klass || !DataBlock) {
    return false;
  }

  ModulePtr mod(new Module("SuperFx"));
  SuperFxDataBlockClass::create(mod, DataBlock);

  root->setMember("superfx", TypePtr(new InternalFunctionValue("superfx", SuperFxPlugin::superfx, DECORATORCALL_INNER)));
  root->setMember("SuperFx", mod);

  return true;
}

// ----------------------------------------------------------------------------
void SuperFxPlugin::release() {
}

// ----------------------------------------------------------------------------
ResultPtr SuperFxPlugin::superfx(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &blocks = parameter.only_blocks();
  if (!blocks.size()) {
    return ResultPtr(new Result());
  }
  
  FMA::plugin::MemoryBlock *block = DataBlockClass::memoryBlock(context);
  if (!block) {
    context->log().error() << "Unable to get memory block from current context";
    return ResultPtr(new Result());
  }

  const TypePtr &dataBlock = SuperFxDataBlockClass::createInstance(context);
  block->declareInType(dataBlock);
  ContextPtr instanceContext(new InstanceContext(context->getInterpreter(), dataBlock->asObject(), "SuperFxDataBlock"));

  std::shared_ptr<DecoratorCall> decoratorCall(std::dynamic_pointer_cast<DecoratorCall>(blocks.front()));
  GroupedParameterList empty;
  ResultPtr result = decoratorCall->callWithContext(instanceContext, empty);

  return result;
}