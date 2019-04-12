#include <fma/core/Class.hpp>
#include <fma/core/Boolean.hpp>
#include <fma/core/SymbolReference.hpp>
#include <fma/core/String.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/types/Object.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/symbol/CalculatedNumber.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/Parameter.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/serialize/SerializeTypes.hpp>
#include <fma/symbol/Reference.hpp>
#include <fma/Project.hpp>

using namespace FMA;
using namespace FMA::core;
using namespace FMA::types;
using namespace FMA::interpret;
using namespace FMA::symbol;

#include <iostream>
#include <sstream>
#include <cmath>

ReferencePtr __emptyReference;

// ----------------------------------------------------------------------------
ClassPtr SymbolReferenceClass::create(const RootModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("SymbolReference", "SymbolReference"));
  klass->extends(ClassObject);
  
  klass->setMember("@FMA_TYPE_ID", TypePtr(new InternalNumberValue(serialize::TYPE_SYMBOLREFERENCE)));

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", SymbolReferenceClass::initialize)));
  proto->setMember("to_sym", TypePtr(new InternalFunctionValue("to_sym", SymbolReferenceClass::to_sym)));
  proto->setMember("to_b", TypePtr(new InternalFunctionValue("to_b", SymbolReferenceClass::to_b)));
  proto->setMember("to_s", TypePtr(new InternalFunctionValue("to_s", SymbolReferenceClass::to_s)));

  proto->setMember("+", TypePtr(new InternalFunctionValue("+", SymbolReferenceClass::op_add)));
  proto->setMember("-", TypePtr(new InternalFunctionValue("-", SymbolReferenceClass::op_sub)));
  proto->setMember("/", TypePtr(new InternalFunctionValue("/", SymbolReferenceClass::op_div)));
  proto->setMember("*", TypePtr(new InternalFunctionValue("*", SymbolReferenceClass::op_mul)));
  proto->setMember("%", TypePtr(new InternalFunctionValue("%", SymbolReferenceClass::op_rem)));
  proto->setMember("&", TypePtr(new InternalFunctionValue("&", SymbolReferenceClass::op_and)));
  proto->setMember("|", TypePtr(new InternalFunctionValue("|", SymbolReferenceClass::op_or)));
  proto->setMember("^", TypePtr(new InternalFunctionValue("^", SymbolReferenceClass::op_xor)));
  proto->setMember("<<", TypePtr(new InternalFunctionValue("<<", SymbolReferenceClass::op_lshift)));
  proto->setMember(">>", TypePtr(new InternalFunctionValue(">>", SymbolReferenceClass::op_rshift)));

  root->setMember("SymbolReference", klass);
  return klass;
}

// ----------------------------------------------------------------------------
ResultPtr SymbolReferenceClass::to_sym(const ContextPtr &context, const GroupedParameterList &) {
  return Result::executed(context, context->self());
}

// ----------------------------------------------------------------------------
ResultPtr SymbolReferenceClass::to_b(const ContextPtr &context, const GroupedParameterList &) {
  return BooleanClass::createInstance(context, true);
}

// ----------------------------------------------------------------------------
ResultPtr SymbolReferenceClass::to_s(const ContextPtr &context, const GroupedParameterList &) {
  return StringClass::createInstance(context, reference(context)->asString());
}

// ----------------------------------------------------------------------------
ResultPtr SymbolReferenceClass::createInstance(const ContextPtr &context, const ReferencePtr &reference) {
  // Resolve a 
  ClassPtr number = context->getRootLevelContext()->resolve("SymbolReference")->asClass();
  if (!number) {
    return ResultPtr(new Result());
  }

  GroupedParameterList parameters;
  ObjectPtr obj = number->createInstance(context, parameters);

  obj->setMember("__value", TypePtr(new InternalReferenceValue(reference)));

  return Result::executed(context, obj);
}

// ----------------------------------------------------------------------------
ResultPtr SymbolReferenceClass::initialize(const ContextPtr &context, const GroupedParameterList &) {
  return Result::executed(context, context->self());
}

// ----------------------------------------------------------------------------
const ReferencePtr &SymbolReferenceClass::reference(const interpret::ContextPtr &context) {
  return reference(context->self());
}

// ----------------------------------------------------------------------------
const ReferencePtr &SymbolReferenceClass::reference(const types::TypePtr &value) {
  TypePtr member = value->getMember("__value");
  if (!member->isInternalObjectOfType("SymbolReference")) {
    return __emptyReference;
  }

  return (dynamic_cast<InternalReferenceValue*>(member.get()))->getValue();
}

// ----------------------------------------------------------------------------
ResultPtr SymbolReferenceClass::calculated(CalculatedNumber::Operand op, const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    return Result::executed(context, context->self());
  }

  ObjectPtr other = args.front()->asObject();
  if (!other) {
    return Result::executed(context, context->self());
  }

  TypePtr result = other->callDirect("to_sym", context, parameter)->get();
  if (result->isUndefined()) {
    context->log().error() << "Unable to do calculation on symbol and " << other->asString();
    return ResultPtr(new Result());
  }

  ReferencePtr left = reference(context);
  ReferencePtr right = reference(result);
  if (!left || !right) {
    context->log().error() << "Unable to do calculation on symbol and " << other->asString();
    return ResultPtr(new Result());
  }

  return createInstance(context, ReferencePtr(new CalculatedNumber(left, op, right)));
}

// ----------------------------------------------------------------------------
ResultPtr SymbolReferenceClass::op_add(const ContextPtr &context, const GroupedParameterList &parameter) {
  return calculated(CalculatedNumber::ADD, context, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr SymbolReferenceClass::op_sub(const ContextPtr &context, const GroupedParameterList &parameter) {
  return calculated(CalculatedNumber::SUB, context, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr SymbolReferenceClass::op_div(const ContextPtr &context, const GroupedParameterList &parameter) {
  return calculated(CalculatedNumber::DIV, context, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr SymbolReferenceClass::op_mul(const ContextPtr &context, const GroupedParameterList &parameter) {
  return calculated(CalculatedNumber::MUL, context, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr SymbolReferenceClass::op_rem(const ContextPtr &context, const GroupedParameterList &parameter) {
  return calculated(CalculatedNumber::REM, context, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr SymbolReferenceClass::op_and(const ContextPtr &context, const GroupedParameterList &parameter) {
  return calculated(CalculatedNumber::AND, context, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr SymbolReferenceClass::op_or(const ContextPtr &context, const GroupedParameterList &parameter) {
  return calculated(CalculatedNumber::OR, context, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr SymbolReferenceClass::op_xor(const ContextPtr &context, const GroupedParameterList &parameter) {
  return calculated(CalculatedNumber::XOR, context, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr SymbolReferenceClass::op_lshift(const ContextPtr &context, const GroupedParameterList &parameter) {
  return calculated(CalculatedNumber::LSHIFT, context, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr SymbolReferenceClass::op_rshift(const ContextPtr &context, const GroupedParameterList &parameter) {
  return calculated(CalculatedNumber::RSHIFT, context, parameter);
}