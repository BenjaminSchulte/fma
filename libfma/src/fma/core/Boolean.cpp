#include <fma/core/Class.hpp>
#include <fma/core/Boolean.hpp>
#include <fma/core/String.hpp>
#include <fma/core/Number.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/types/Object.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/Parameter.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/serialize/SerializeTypes.hpp>
#include <fma/Project.hpp>

using namespace FMA;
using namespace FMA::core;
using namespace FMA::types;
using namespace FMA::interpret;

#include <iostream>
#include <sstream>

// ----------------------------------------------------------------------------
ClassPtr BooleanClass::create(const RootModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("Boolean", "Boolean"));
  klass->extends(ClassObject);

  klass->setMember("@FMA_TYPE_ID", TypePtr(new InternalNumberValue(serialize::TYPE_BOOLEAN)));

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", BooleanClass::initialize)));
  proto->setMember("to_b", TypePtr(new InternalFunctionValue("to_b", BooleanClass::to_b)));
  proto->setMember("to_n", TypePtr(new InternalFunctionValue("to_n", BooleanClass::to_n)));
  proto->setMember("to_s", TypePtr(new InternalFunctionValue("to_s", BooleanClass::to_s)));

  proto->setMember("==", TypePtr(new InternalFunctionValue("==", BooleanClass::op_eq)));

  root->setMember("Boolean", klass);
  return klass;
}

// ----------------------------------------------------------------------------
ResultPtr BooleanClass::createInstance(const ContextPtr &context, bool value) {
  // Resolve a 
  ClassPtr number = context->getRootLevelContext()->resolve("Boolean")->asClass();
  if (!number) {
    return ResultPtr(new Result());
  }

  GroupedParameterList parameters;
  parameters.push_back(TypePtr(new InternalNumberValue(value ? 1 : 0)));
  return Result::executed(context, number->createInstance(context, parameters));
}

// ----------------------------------------------------------------------------
ResultPtr BooleanClass::initialize(const ContextPtr &context, const GroupedParameterList &parameter) {
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
ResultPtr BooleanClass::op_eq(const ContextPtr &context, const GroupedParameterList& params) {
  const TypeList &args = params.only_args();
  if (!args.size()) { return to_s(context, params); }

  TypePtr right = args.front();
  if (!right->hasMember("to_b")) {
    context->log().error() << "Unsupported operand for boolean calculation: " << right->asString();
    return to_b(context, params);
  }

  return BooleanClass::createInstance(context, context->self()->convertToBoolean(context) == right->convertToBoolean(context));
}

// ----------------------------------------------------------------------------
ResultPtr BooleanClass::to_b(const ContextPtr &context, const GroupedParameterList&) {
  return Result::executed(context, context->self());
}

// ----------------------------------------------------------------------------
ResultPtr BooleanClass::to_n(const ContextPtr &context, const GroupedParameterList&) {
  TypePtr number = context->self()->getMember("__value");
  if (!number->isInternalObjectOfType("Number")) {
    return NumberClass::createInstance(context, 0);
  }

  InternalNumberValue *value = dynamic_cast<InternalNumberValue*>(number.get());
  return NumberClass::createInstance(context, value->getValue());
}

// ----------------------------------------------------------------------------
ResultPtr BooleanClass::to_s(const ContextPtr &context, const GroupedParameterList&) {
  int64_t value = context->self()->convertToNumber(context);

  return StringClass::createInstance(context, value ? "true" : "false");
}

// ----------------------------------------------------------------------------
void BooleanClass::serializeObject(const TypePtr &self, const serialize::SerializeObjectPtr &object) {
  TypePtr number = self->getMember("__value");
  uint8_t value = 0;
  
  if (number->isInternalObjectOfType("Number")) {
    InternalNumberValue *numberValue = dynamic_cast<InternalNumberValue*>(number.get());
    value = numberValue->getValue();
  }

  object->write(&value, sizeof(value));
}

// ----------------------------------------------------------------------------
bool BooleanClass::deserializeObject(const serialize::DeserializeObjectPtr &object) {
  object->allowBufferReadWithoutObject();

  uint8_t value;
  if (object->read(&value, sizeof(value) != sizeof(value))) {
    return false;
  }

  object->store(BooleanClass::createInstance(object->getContext(), !!value)->get());
  return true;
}

// ----------------------------------------------------------------------------