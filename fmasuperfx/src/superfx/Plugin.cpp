#include <superfx/Plugin.hpp>
#include <superfx/SuperFxDataBlockClass.hpp>
#include <superfx/Register.hpp>
#include <fma/Project.hpp>
#include <fma/core/String.hpp>
#include <fma/core/DataBlock.hpp>
#include <fma/plugin/MemoryPluginAdapter.hpp>
#include <fma/types/Module.hpp>
#include <fma/types/Class.hpp>
#include <fma/types/Object.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/InstanceContext.hpp>
#include <fma/interpret/Interpreter.hpp>
#include <fma/interpret/ProjectContext.hpp>

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

  Interpreter interpreter(project);
  ContextPtr context(new ProjectContext(&interpreter));
  RegisterClass::create(root, klass);
  root->setMember("R0", RegisterClass::createInstance(context, "R0", 0)->get());
  root->setMember("R1", RegisterClass::createInstance(context, "R1", 1)->get());
  root->setMember("R2", RegisterClass::createInstance(context, "R2", 2)->get());
  root->setMember("R3", RegisterClass::createInstance(context, "R3", 3)->get());
  root->setMember("R4", RegisterClass::createInstance(context, "R4", 4)->get());
  root->setMember("R5", RegisterClass::createInstance(context, "R5", 5)->get());
  root->setMember("R6", RegisterClass::createInstance(context, "R6", 6)->get());
  root->setMember("R7", RegisterClass::createInstance(context, "R7", 7)->get());
  root->setMember("R8", RegisterClass::createInstance(context, "R8", 8)->get());
  root->setMember("R9", RegisterClass::createInstance(context, "R9", 9)->get());
  root->setMember("R10", RegisterClass::createInstance(context, "R10", 10)->get());
  root->setMember("R11", RegisterClass::createInstance(context, "R11", 11)->get());
  root->setMember("R12", RegisterClass::createInstance(context, "R12", 12)->get());
  root->setMember("R13", RegisterClass::createInstance(context, "R13", 13)->get());
  root->setMember("R14", RegisterClass::createInstance(context, "R14", 14)->get());
  root->setMember("R15", RegisterClass::createInstance(context, "R15", 15)->get());

  return true;
}

// ----------------------------------------------------------------------------
bool SuperFxPlugin::createRegister(const RootModulePtr &root, const char *name) {
  ClassPtr reg = root->getMember("Register")->asClass();
  root->dump("");
  if (!reg) {
    project->log().error() << "Unable to find class 'Register'";
    return false;
  }

  Interpreter interpreter(project);
  ContextPtr context(new ProjectContext(&interpreter));

  GroupedParameterList parameters;
  parameters.push_back(StringClass::createInstance(context, name)->get());
  
  ObjectPtr obj = reg->createInstance(context, parameters);
  root->setMember(name, obj);

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

  ContextPtr oldContext = instanceContext->getInterpreter()->getGlobalContext();
  instanceContext->getInterpreter()->setGlobalContext(instanceContext);
  ResultPtr result = decoratorCall->callWithContext(instanceContext, empty);
  instanceContext->getInterpreter()->setGlobalContext(oldContext);
  return result;
}