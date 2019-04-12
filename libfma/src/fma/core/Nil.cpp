#include <fma/core/Class.hpp>
#include <fma/core/Nil.hpp>
#include <fma/core/String.hpp>
#include <fma/core/Boolean.hpp>
#include <fma/core/Number.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/types/Object.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/Parameter.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/Project.hpp>

using namespace FMA;
using namespace FMA::core;
using namespace FMA::types;
using namespace FMA::interpret;
using namespace FMA::serialize;

#include <iostream>
#include <sstream>

// ----------------------------------------------------------------------------
ClassPtr NilClass::create(const RootModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("Nil", "Nil"));
  klass->extends(ClassObject);

  klass->setMember("@FMA_TYPE_ID", TypePtr(new InternalNumberValue(TYPE_NIL)));

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", NilClass::initialize)));
  proto->setMember("nil?", TypePtr(new InternalFunctionValue("nil?", NilClass::nil_qm)));
  proto->setMember("to_b", TypePtr(new InternalFunctionValue("to_b", NilClass::to_b)));
  proto->setMember("to_n", TypePtr(new InternalFunctionValue("to_n", NilClass::to_n)));
  proto->setMember("to_s", TypePtr(new InternalFunctionValue("to_s", NilClass::to_s)));

  proto->setMember("==", TypePtr(new InternalFunctionValue("==", NilClass::op_eq)));

  root->setMember("Nil", klass);
  return klass;
}

// ----------------------------------------------------------------------------
ResultPtr NilClass::createInstance(const ContextPtr &context) {
  // Resolve a 
  ClassPtr nil = context->getRootLevelContext()->resolve("Nil")->asClass();
  if (!nil) {
    return ResultPtr(new Result());
  }

  GroupedParameterList parameters;
  return Result::executed(context, nil->createInstance(context, parameters));
}

// ----------------------------------------------------------------------------
ResultPtr NilClass::initialize(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  
  switch (args.size()) {
    case 0:
      context->self()->setMember("__value", TypePtr(new InternalNumberValue(0)));
      break;
    
    case 1:
    default:
      context->self()->setMember("__value", args.front());
      break;
  }

  return Result::executed(context, context->self());
}

// ----------------------------------------------------------------------------
ResultPtr NilClass::op_eq(const ContextPtr &context, const GroupedParameterList& params) {
  const TypeList &args = params.only_args();
  if (!args.size()) { return to_s(context, params); }

  TypePtr right = args.front();
  if (!right->hasMember("nil?")) {
    return BooleanClass::createInstance(context, false);
  }

  return BooleanClass::createInstance(context, right->getMember("nil?")->call(context, params)->get()->convertToBoolean(context));
}

// ----------------------------------------------------------------------------
ResultPtr NilClass::nil_qm(const ContextPtr &context, const GroupedParameterList&) {
  return BooleanClass::createInstance(context, true);
}

// ----------------------------------------------------------------------------
ResultPtr NilClass::to_b(const ContextPtr &context, const GroupedParameterList&) {
  return BooleanClass::createInstance(context, false);
}

// ----------------------------------------------------------------------------
ResultPtr NilClass::to_n(const ContextPtr &context, const GroupedParameterList&) {
  return NumberClass::createInstance(context, 0);
}

// ----------------------------------------------------------------------------
ResultPtr NilClass::to_s(const ContextPtr &context, const GroupedParameterList&) {
  return StringClass::createInstance(context, "nil");
}

// ----------------------------------------------------------------------------
void NilClass::serializeObject(const TypePtr &, const serialize::SerializeObjectPtr &) {
  // Nothing to serialize here
}

// ----------------------------------------------------------------------------
bool NilClass::deserializeObject(const serialize::DeserializeObjectPtr &object) {
  object->store(NilClass::createInstance(object->getContext())->get());
  return true;
}

// ----------------------------------------------------------------------------