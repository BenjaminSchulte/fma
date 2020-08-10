#include <fma/core/Class.hpp>
#include <fma/core/Boolean.hpp>
#include <fma/core/TypedNumber.hpp>
#include <fma/core/String.hpp>
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
#include <cmath>

// ----------------------------------------------------------------------------
ClassPtr TypedNumberClass::create(const RootModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("TypedNumber", "TypedNumber"));
  klass->extends(ClassObject);
  
  klass->setMember("@FMA_TYPE_ID", TypePtr(new InternalNumberValue(serialize::TYPE_TYPEDNUMBER)));

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", TypedNumberClass::initialize)));
  proto->setMember("+", TypePtr(new InternalFunctionValue("+", TypedNumberClass::op_add)));
  proto->setMember("-", TypePtr(new InternalFunctionValue("-", TypedNumberClass::op_sub)));
  proto->setMember("/", TypePtr(new InternalFunctionValue("/", TypedNumberClass::op_div)));
  proto->setMember("*", TypePtr(new InternalFunctionValue("*", TypedNumberClass::op_mul)));
  proto->setMember("%", TypePtr(new InternalFunctionValue("%", TypedNumberClass::op_rem)));
  proto->setMember("&", TypePtr(new InternalFunctionValue("&", TypedNumberClass::op_and)));
  proto->setMember("|", TypePtr(new InternalFunctionValue("|", TypedNumberClass::op_or)));
  proto->setMember("^", TypePtr(new InternalFunctionValue("^", TypedNumberClass::op_xor)));
  proto->setMember("<<", TypePtr(new InternalFunctionValue("<<", TypedNumberClass::op_lshift)));
  proto->setMember(">>", TypePtr(new InternalFunctionValue(">>", TypedNumberClass::op_rshift)));

  root->setMember("TypedNumber", klass);
  return klass;
}

// ----------------------------------------------------------------------------
ResultPtr TypedNumberClass::op_add(const ContextPtr &context, const GroupedParameterList &parameter) {
  return childOpCall("+", context, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr TypedNumberClass::op_sub(const ContextPtr &context, const GroupedParameterList &parameter) {
  return childOpCall("-", context, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr TypedNumberClass::op_div(const ContextPtr &context, const GroupedParameterList &parameter) {
  return childOpCall("/", context, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr TypedNumberClass::op_mul(const ContextPtr &context, const GroupedParameterList &parameter) {
  return childOpCall("*", context, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr TypedNumberClass::op_rem(const ContextPtr &context, const GroupedParameterList &parameter) {
  return childOpCall("%", context, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr TypedNumberClass::op_and(const ContextPtr &context, const GroupedParameterList &parameter) {
  return childOpCall("&", context, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr TypedNumberClass::op_or(const ContextPtr &context, const GroupedParameterList &parameter) {
  return childOpCall("|", context, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr TypedNumberClass::op_xor(const ContextPtr &context, const GroupedParameterList &parameter) {
  return childOpCall("^", context, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr TypedNumberClass::op_lshift(const ContextPtr &context, const GroupedParameterList &parameter) {
  return childOpCall("<<", context, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr TypedNumberClass::op_rshift(const ContextPtr &context, const GroupedParameterList &parameter) {
  return childOpCall(">>", context, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr TypedNumberClass::childOpCall(const std::string &op, const ContextPtr &context, const GroupedParameterList &params) {
  // Resolve a 
  ClassPtr number = context->getRootLevelContext()->resolve("TypedNumber")->asClass();
  if (!number) {
    return ResultPtr(new Result());
  }

  GroupedParameterList parameters;
  auto result(context->self()->getDirectMember("number")->callDirect(op, context, params)->get());
  parameters.push_back(result);
  parameters.push_back(context->self()->getDirectMember("type"));
  return Result::executed(context, number->createInstance(context, parameters));
}

// ----------------------------------------------------------------------------
ResultPtr TypedNumberClass::createInstance(const ContextPtr &context, const TypePtr &value, const std::string &type) {
  // Resolve a 
  ClassPtr number = context->getRootLevelContext()->resolve("TypedNumber")->asClass();
  if (!number) {
    return ResultPtr(new Result());
  }

  GroupedParameterList parameters;
  parameters.push_back(value);
  parameters.push_back(StringClass::createInstance(context, type)->get());
  return Result::executed(context, number->createInstance(context, parameters));
}


// ----------------------------------------------------------------------------
ResultPtr TypedNumberClass::initialize(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (args.size() != 2) {
    return ResultPtr(new Result());
  }
  
  context->self()->setMember("number", args.front());
  context->self()->setMember("type", args.back());

  return Result::executed(context, context->self());
}
