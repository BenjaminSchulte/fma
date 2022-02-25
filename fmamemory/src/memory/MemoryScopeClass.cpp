#include <memory/MemoryScopeClass.hpp>
#include <memory/MemoryMapClass.hpp>
#include <memory/MemoryAdapter.hpp>
#include <memory/MemoryMap.hpp>
#include <memory/MemoryScope.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/types/Object.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/core/SymbolReference.hpp>
#include <fma/Project.hpp>

#include <iostream>

using namespace FMA;
using namespace FMA::memory;
using namespace FMA::types;
using namespace FMA::core;
using namespace FMA::interpret;

typedef InternalObject<MemoryScopePtr> InternalMemoryScopeValue;

namespace FMA {
namespace types {
  template<> const char *getInternalObjectName<MemoryScopePtr>() { return "MemoryScope"; }
}
}

namespace {
  MemoryScopePtr emptyMemoryScope;
}

// ----------------------------------------------------------------------------
ClassPtr MemoryScopeClass::create(const RootModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("MemoryScope", "MemoryScope"));
  klass->extends(ClassObject);

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", MemoryScopeClass::initialize)));
  proto->setMember("include", TypePtr(new InternalFunctionValue("include", MemoryScopeClass::include)));
  proto->setMember("allow", TypePtr(new InternalFunctionValue("allow", MemoryScopeClass::allow)));
  proto->setMember("deny", TypePtr(new InternalFunctionValue("deny", MemoryScopeClass::deny)));
  proto->setMember("to_sym", TypePtr(new InternalFunctionValue("to_sym", MemoryScopeClass::to_sym)));

  root->setMember("MemoryScope", klass);
  return klass;
}

// ----------------------------------------------------------------------------
ResultPtr MemoryScopeClass::initialize(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  const TypeMap &kwargs = parameter.only_kwargs();
  TypeMap::const_iterator it;
  
  GroupedParameterList emptyParameter;

  bool shared = false;
  if ((it = kwargs.find("shared")) != kwargs.end()) {
    shared = it->second->convertToBoolean(context);
  }

  if ((it = kwargs.find("in")) == kwargs.end()) {
    context->log().error() << "Missing argument 'in' for scope declaration";
    return ResultPtr(new Result());
  }

  TypePtr parent = it->second;
  MemoryScopePtr scope;
  if (parent->isObjectOfType("MemoryMap")) {
    scope = MemoryScopePtr(MemoryMapClass::asMemoryMap(context->getProject(), parent)->createMemoryScope(shared));
  } else if (parent->isObjectOfType("MemoryScope")) {
    scope = MemoryScopePtr(asMemoryScope(context->getProject(), parent)->createScope(shared));
  } else if (parent->callDirect("nil?", context, emptyParameter)->get()->convertToBoolean(context)) {
    if ((it = kwargs.find("for")) == kwargs.end()) {
      context->log().error() << "Missing argument 'for' for scope loose declaration";
      return ResultPtr(new Result());
    }

    TypePtr targetMap = it->second;
    if (!targetMap->isObjectOfType("MemoryMap")) {
      context->log().error() << "Argument 'for' for scope loose declaration must be memory map";
      return ResultPtr(new Result());
    }

    scope = MemoryScopePtr(MemoryMapClass::asMemoryMap(context->getProject(), targetMap)->createLooseMemoryScope(shared));
  } else {
    context->log().error() << "Invalid 'in' value for scope: " << parent->asString();
    return ResultPtr(new Result());
  }

  if (args.size()) {
    scope->setNameHint(args.front()->convertToString(context));
  }

  it = kwargs.find("size");
  if (it == kwargs.end()) {
    it = kwargs.find("length");
  }
  if (it != kwargs.end()) {
    scope->setSize(it->second->convertToNumber(context));
  }

  std::ostringstream os;
  os << context->getNameHint() << "." << scope->getNameHint();
  scope->setReference(context->getProject()->getMemoryAdapter()->createReference(os.str()));

  context->self()->setMember("__memory_scope", TypePtr(new InternalMemoryScopeValue(scope)));
  scope->locations().allow(context, parameter);

  return Result::executed(context, context->self());
}

// ----------------------------------------------------------------------------
ResultPtr MemoryScopeClass::include(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  const MemoryScopePtr &self = asMemoryScope(context);

  for (auto &ram : args) {
    if (!ram->isObjectOfType("MemoryScope")) {
      context->log().error() << "Parameter for 'include' must be of type MemoryScope";
      continue;
    }

    self->addChild(asMemoryScope(context->getProject(), ram));
  }

  return Result::executed(context, context->self());
}

// ----------------------------------------------------------------------------
ResultPtr MemoryScopeClass::allow(const ContextPtr &context, const GroupedParameterList &parameter) {
  asMemoryScope(context)->locations().allow(context, parameter);
  return Result::executed(context, context->self());
}

// ----------------------------------------------------------------------------
ResultPtr MemoryScopeClass::deny(const ContextPtr &context, const GroupedParameterList &parameter) {
  asMemoryScope(context)->locations().deny(context, parameter);
  return Result::executed(context, context->self());
}

// ----------------------------------------------------------------------------
ResultPtr MemoryScopeClass::to_sym(const ContextPtr &context, const GroupedParameterList &) {
  const MemoryScopePtr &alloc = asMemoryScope(context);
  if (!alloc) {
    return ResultPtr(new Result());
  }

  return SymbolReferenceClass::createInstance(context, alloc->getReference());
}

// ----------------------------------------------------------------------------
const MemoryScopePtr &MemoryScopeClass::asMemoryScope(const interpret::ContextPtr &context) {
  return asMemoryScope(context->getProject(), context->self());
}

// ----------------------------------------------------------------------------
const MemoryScopePtr &MemoryScopeClass::asMemoryScope(Project *project, const types::TypePtr &value) {
  const TypePtr &obj = value->getMember("__memory_scope");
  if (!obj || !obj->isInternalObjectOfType("MemoryScope")) {
    project->log().error() << "Unable to access internal memory scope";
    return emptyMemoryScope;
  }

  return std::dynamic_pointer_cast<InternalMemoryScopeValue>(obj)->getValue();
}