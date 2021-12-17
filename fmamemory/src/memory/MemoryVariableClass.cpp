#include <memory/MemoryVariableClass.hpp>
#include <memory/MemoryVariablePointerClass.hpp>
#include <memory/MemoryClassMembers.hpp>
#include <memory/MemoryAllocation.hpp>
#include <memory/MemoryScopeClass.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/core/SymbolReference.hpp>
#include <fma/core/Boolean.hpp>
#include <fma/core/Number.hpp>
#include <fma/Project.hpp>

#include <iostream>

using namespace FMA;
using namespace FMA::memory;
using namespace FMA::types;
using namespace FMA::interpret;
using namespace FMA::core;

typedef InternalObject<MemoryAllocationPtr> InternalMemoryAllocationValue;

namespace FMA {
namespace types {
  template<> const char *getInternalObjectName<MemoryAllocationPtr>() { return "MemoryAllocation"; }
}
}

MemoryAllocationPtr __emptyMemoryAllocation;

// ----------------------------------------------------------------------------
ClassPtr MemoryVariableClass::create(const RootModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("MemoryVariable", "MemoryVariable"));
  klass->extends(ClassObject);

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", MemoryVariableClass::initialize)));
  proto->setMember("size", TypePtr(new InternalFunctionValue("to_sym", MemoryVariableClass::size)));
  proto->setMember("to_sym", TypePtr(new InternalFunctionValue("to_sym", MemoryVariableClass::to_sym)));
  proto->setMember("[]", TypePtr(new InternalFunctionValue("[]", MemoryVariableClass::op_index)));
  proto->setMember(".", TypePtr(new InternalFunctionValue(".", MemoryVariableClass::op_index)));

  root->setMember("MemoryVariable", klass);
  return klass;
}

// ----------------------------------------------------------------------------
ResultPtr MemoryVariableClass::to_sym(const ContextPtr &context, const GroupedParameterList &) {
  const MemoryAllocationPtr &alloc = asMemoryAllocation(context);
  if (!alloc) {
    context->log().fatal() << "Could not access memory allocation on object " << context->self()->asString();
    return ResultPtr(new Result());
  }

  return SymbolReferenceClass::createInstance(context, alloc->getReference());
}

// ----------------------------------------------------------------------------
uint8_t MemoryVariableClass::getInternalTypeSize(const std::string &str) {
  if (str == "uint8") { return 1; }
  else if (str == "uint16") { return 2; }
  else if (str == "uint24") { return 3; }
  else if (str == "uint32") { return 4; }
  else if (str == "int8") { return 1; }
  else if (str == "int16") { return 2; }
  else if (str == "int24") { return 3; }
  else if (str == "int32") { return 4; }

  return 0;
}

// ----------------------------------------------------------------------------
ResultPtr MemoryVariableClass::initialize(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  const TypeMap &kwargs = parameter.only_kwargs();
  TypeMap::const_iterator it;

  if ((it = kwargs.find("in")) == kwargs.end()) {
    context->log().error() << "Missing argument 'in' for declaration";
    return ResultPtr(new Result());
  }

  TypePtr parent = it->second;
  MemoryScopePtr scope;
  TypePtr resultValue = context->self();
  if (parent->isObjectOfType("MemoryScope")) {
    scope = MemoryScopePtr(MemoryScopeClass::asMemoryScope(context->getProject(), parent)->createScope(false));
  } else {
    context->log().error() << "Invalid 'in' value for declaration: " << parent->asString();
    return ResultPtr(new Result());
  }

  if (args.size()) {
    scope->setNameHint(args.front()->convertToString(context));
  }

  std::ostringstream os;
  scope->setReference(context->getProject()->getMemoryAdapter()->createReference(scope->getNameHint()));

  if ((it = kwargs.find("as")) == kwargs.end()) {
    context->log().error() << "Missing argument 'as' for declaration";
    return ResultPtr(new Result());
  }
  
  const TypePtr &as = it->second;
  bool isValidSize = false;
  if (as->isObjectOfType("String")) {
    uint8_t size = getInternalTypeSize(as->convertToString(context));
    if (size > 0) {
      isValidSize = true;
      scope->setSize(size);
    }
  }

  if (isValidSize) {
  } else if (as->isClass()) {
    scope->setClass(as->asClass());
  } else if (as->hasMember("to_n")) {
    scope->setSize(as->convertToNumber(context));
  } else if (as->hasMember("size")) {
    scope->setSize(as->convertToNumber(context, "size"));
  } else {
    context->log().error() << "Unsupported `as` parameter";
  }

  it = kwargs.find("size");
  if (it == kwargs.end()) it = kwargs.find("length");
  if (it != kwargs.end()) {
    scope->setArraySize(it->second->convertToNumber(context));
  }

  context->self()->setMember("__memory_declaration", TypePtr(new InternalMemoryAllocationValue(scope)));
  scope->locations().allow(context, parameter);

  return Result::executed(context, resultValue);
}

// ----------------------------------------------------------------------------
ResultPtr MemoryVariableClass::size(const ContextPtr &context, const GroupedParameterList &) {
  const MemoryAllocationPtr &memory = asMemoryAllocation(context);
  return NumberClass::createInstance(context, memory->getSize());
}

// ----------------------------------------------------------------------------
ResultPtr MemoryVariableClass::op_index(const ContextPtr &context, const GroupedParameterList &parameter) {
  const MemoryAllocationPtr &memory = asMemoryAllocation(context);
  if (memory->isArray()) {
    return op_index_array(memory, context, parameter);
  } else {
    return op_index_member(memory, context, parameter);
  }
}

// ----------------------------------------------------------------------------
ResultPtr MemoryVariableClass::op_index_array(const MemoryAllocationPtr &memory, const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    return ResultPtr(new Result());
  }

  uint64_t offset = args.front()->convertToNumber(context) * memory->getItemSize();
  
  GroupedParameterList par;
  par.push_back(context->self());
  par.push_back(Parameter("in_array", BooleanClass::createInstance(context, true)->get()));
  par.push_back(Parameter("base", NumberClass::createInstance(context, offset)->get()));
  return MemoryVariablePointerClass::createInstance(context, par);
}

// ----------------------------------------------------------------------------
ResultPtr MemoryVariableClass::op_index_member(const MemoryAllocationPtr &memory, const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    return ResultPtr(new Result());
  }

  const ClassPtr &klass = memory->getClass();
  if (!klass) {
    return ResultPtr(new Result());
  }

  std::string memberName = args.front()->convertToString(context);
  const auto &member = MemoryClassMembers::getClassMembers(klass);

  uint64_t offset = member->getOffsetOf(memberName);
  TypePtr typeHint = member->getTypeHintOf(memberName);
/*
  ParameterList par;
  par.push_back(context->self());
  par.push_back(Parameter("in_member", BooleanClass::createInstance(context, true)->get()));
  par.push_back(Parameter("base", NumberClass::createInstance(context, offset)->get()));
  
  const auto &result = MemoryVariablePointerClass::createInstance(context, par)->get();

  // TODO: Only return symbol if the result is a basic type
  ParameterList emptyParameter;
  return result->callDirect("to_sym", context, emptyParameter);*/

  GroupedParameterList noParameter;
  GroupedParameterList rightParameter;
  rightParameter.push_back(NumberClass::createInstance(context, offset)->get());
  
  ResultPtr result = to_sym(context, noParameter)->get()->callDirect("+", context, rightParameter);
  if (typeHint) {
    result->get()->setMember("__typehint", typeHint);
  }
  return result;
}

// ----------------------------------------------------------------------------
const MemoryAllocationPtr &MemoryVariableClass::asMemoryAllocation(const interpret::ContextPtr &context) {
  return asMemoryAllocation(context->getProject(), context->self());
}

// ----------------------------------------------------------------------------
const MemoryAllocationPtr &MemoryVariableClass::asMemoryAllocation(Project *project, const types::TypePtr &value) {
  const TypePtr &obj = value->getMember("__memory_declaration");
  if (!obj || !obj->isInternalObjectOfType("MemoryAllocation")) {
    project->log().error() << "Unable to access internal memory allocation";
    return __emptyMemoryAllocation;
  }

  return std::dynamic_pointer_cast<InternalMemoryAllocationValue>(obj)->getValue();
}