#include <memory/MemoryVariableClass.hpp>
#include <memory/MemoryVariablePointerClass.hpp>
#include <memory/MemoryClassMembers.hpp>
#include <memory/ArrayMemberClassInstance.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/InstanceContext.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/types/Object.hpp>
#include <fma/core/Boolean.hpp>
#include <fma/core/Number.hpp>
#include <fma/Project.hpp>

#include <iostream>

using namespace FMA;
using namespace FMA::memory;
using namespace FMA::types;
using namespace FMA::interpret;
using namespace FMA::core;

// ----------------------------------------------------------------------------
ClassPtr MemoryVariablePointerClass::create(const RootModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("MemoryVariable$Pointer", "MemoryVariable$Pointer"));
  klass->extends(ClassObject);

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", MemoryVariablePointerClass::initialize)));
  proto->setMember("to_sym", TypePtr(new InternalFunctionValue("to_sym", MemoryVariablePointerClass::to_sym)));
  proto->setMember(".", TypePtr(new InternalFunctionValue(".", MemoryVariablePointerClass::op_index)));
  proto->setMember("[]", TypePtr(new InternalFunctionValue("[]", MemoryVariablePointerClass::op_index)));

  root->setMember("MemoryVariable$Pointer", klass);
  return klass;
}

// ----------------------------------------------------------------------------
ResultPtr MemoryVariablePointerClass::createInstance(const ContextPtr &context, const GroupedParameterList &parameter) {
  ClassPtr MemoryVariable = context->getRootLevelContext()->getMember("MemoryVariable$Pointer")->get()->asClass();
  if (!MemoryVariable) {
    return ResultPtr(new Result());
  }

  return Result::executed(context, MemoryVariable->createInstance(context, parameter));
}

// ----------------------------------------------------------------------------
ResultPtr MemoryVariablePointerClass::initialize(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  const TypeMap &kwargs = parameter.only_kwargs();
  TypeMap::const_iterator it;
  if (!args.size()) {
    return Result::executed(context, context->self());
  }

  const auto &self = context->self();
  self->setMember("parent", args.front());

  if ((it = kwargs.find("in_array")) != kwargs.end()) {
    self->setMember("in_array", it->second);
  } else {
    self->setMember("in_array", BooleanClass::createInstance(context, false)->get());
  }

  if ((it = kwargs.find("in_member")) != kwargs.end()) {
    self->setMember("in_member", it->second);
  } else {
    self->setMember("in_member", BooleanClass::createInstance(context, false)->get());
  }

  if ((it = kwargs.find("base")) != kwargs.end()) {
    self->setMember("base", it->second);
  } else {
    self->setMember("base", NumberClass::createInstance(context, 0)->get());
  }


  return Result::executed(context, context->self());
}

// ----------------------------------------------------------------------------
ResultPtr MemoryVariablePointerClass::op_index(const ContextPtr &context, const GroupedParameterList &parameter) {
  const auto &self = context->self();
  TypePtr parent = self->getMember("parent");
  const MemoryAllocationPtr &memory = MemoryVariableClass::asMemoryAllocation(context->getProject(), parent);

  if (memory->isArray() && !self->getMember("in_array")->convertToBoolean(context)) {
    std::cerr << "TODO: Access array" << std::endl;
    return ResultPtr(new Result());
  }

  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    return ResultPtr(new Result());
  }

  if (!memory->isClass()) {
    return ResultPtr(new Result());
  }

  std::string memberName = args.front()->convertToString(context);
  if (memory->getClass()->hasPrototypeMember(memberName)) {
    return Result::executed(
      context,
      TypePtr(new ArrayMemberClassInstance(context, memory->getClass()->getPrototypeMember(memberName)))
    );

    return ResultPtr(new Result());
  } else {
    const auto &members = MemoryClassMembers::getClassMembers(memory->getClass());
  
    uint64_t offset = members->getOffsetOf(memberName);

    GroupedParameterList noParameter;
    GroupedParameterList rightParameter;
    rightParameter.push_back(NumberClass::createInstance(context, offset)->get());
    return to_sym(context, noParameter)->get()->callDirect("+", context, rightParameter);
  }
}

// ----------------------------------------------------------------------------
ResultPtr MemoryVariablePointerClass::to_sym(const ContextPtr &context, const GroupedParameterList &parameter) {
  const auto &self = context->self();
  const auto &parent = self->getMember("parent")->asObject();

  TypePtr sym = parent->callDirect("to_sym", context, parameter)->get();

  GroupedParameterList rightParams;
  rightParams.push_back(self->getMember("base"));

  return sym->callDirect("+", context, rightParams);
}
