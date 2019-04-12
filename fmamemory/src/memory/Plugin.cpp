#include <memory/Plugin.hpp>
#include <memory/MemoryMapClass.hpp>
#include <memory/MemoryDeclarationClass.hpp>
#include <memory/MemoryScopeClass.hpp>
#include <memory/MemoryVariableClass.hpp>
#include <memory/MemoryVariablePointerClass.hpp>
#include <memory/MemoryAdapter.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/types/Class.hpp>
#include <fma/types/Object.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/core/Number.hpp>
#include <fma/core/String.hpp>
#include <fma/interpret/InstanceContext.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/interpret/Interpreter.hpp>
#include <fma/Project.hpp>

#include <iostream>

using namespace FMA;
using namespace FMA::memory;
using namespace FMA::interpret;
using namespace FMA::types;
using namespace FMA::plugin;
using namespace FMA::core;

// ----------------------------------------------------------------------------
MemoryPlugin::MemoryPlugin(Project *project)
  : MemoryManagerPlugin(project)
{
}

// ----------------------------------------------------------------------------
bool MemoryPlugin::initialize() {
  const RootModulePtr &root = project->root();
  ClassPtr Klass = root->getMember("Class")->asClass();
  if (!Klass) {
    project->log().error() << "Could not resolve type 'Class'";
    return false;
  }

  MemoryMapClass::create(root, Klass);
  MemoryVariablePointerClass::create(root, Klass);
  
  ClassPtr MemoryDeclaration = MemoryDeclarationClass::create(root, Klass);
  MemoryScopeClass::create(root, MemoryDeclaration);
  MemoryVariableClass::create(root, MemoryDeclaration);

  root->setMember("scope", TypePtr(new InternalFunctionValue("scope", MemoryPlugin::scope)));
  root->setMember("declare", TypePtr(new InternalFunctionValue("declare", MemoryPlugin::declare)));
  root->setMember("locate_at", TypePtr(new InternalFunctionValue("locate_at", MemoryPlugin::locate_at, DECORATORCALL_INNER)));

  Klass->setMember("pointer", TypePtr(new InternalFunctionValue("pointer", MemoryPlugin::class_pointer)));

  return true;
}

// ----------------------------------------------------------------------------
MemoryPluginAdapter *MemoryPlugin::createAdapter() {
  return new MemoryAdapter(project);
}

// ----------------------------------------------------------------------------
void MemoryPlugin::releaseAdapter(MemoryPluginAdapter *adapter) {
  delete adapter;
}

// ----------------------------------------------------------------------------
ResultPtr MemoryPlugin::class_pointer(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  std::string pointerType = "uint16";
  if (args.size()) {
    pointerType = args.front()->convertToString(context);
  }

  TypePtr typeStr = StringClass::createInstance(context, pointerType)->get();
  typeStr->setMember("__typehint", context->self());
  return ResultPtr(new Result(context, typeStr));
}

// ----------------------------------------------------------------------------
ResultPtr MemoryPlugin::locate_at(const ContextPtr &context, const GroupedParameterList &parameter) {
  const ContextPtr &global = context->getInterpreter()->getGlobalContext();
  if (!global) {
    context->log().error() << "Could not access global context.";
    return ResultPtr(new Result());
  }

  const ObjectPtr &self = global->self()->asObject();
  if (!self) {
    context->log().error() << "Could not access self.";
    return ResultPtr(new Result());
  }
  
  self->callDirect("override_located_at", context, parameter);
  //ContextPtr callContext(new InstanceContext(context->getInterpreter(), self, "allow_located_at"));
  //self->getMember("allow_located_at")->call(callContext, parameter);

  const TypeList &blocks = parameter.only_blocks();
  if (!blocks.size()) {
    return ResultPtr(new Result());
  }

  GroupedParameterList empty;
  return blocks.front()->call(context, empty);
}

// ----------------------------------------------------------------------------
ResultPtr MemoryPlugin::scope(const ContextPtr &context, const GroupedParameterList &parameter) {
  return dynamic_cast<MemoryAdapter*>(context->getProject()->getMemoryAdapter())->createScope(context, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr MemoryPlugin::declare(const ContextPtr &context, const GroupedParameterList &parameter) {
  return dynamic_cast<MemoryAdapter*>(context->getProject()->getMemoryAdapter())->createDeclaration(context, parameter);
}

// ----------------------------------------------------------------------------
void MemoryPlugin::release() {

}

// ----------------------------------------------------------------------------