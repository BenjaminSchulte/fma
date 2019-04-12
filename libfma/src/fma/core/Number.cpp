#include <fma/core/Class.hpp>
#include <fma/core/Boolean.hpp>
#include <fma/core/Number.hpp>
#include <fma/core/String.hpp>
#include <fma/core/SymbolReference.hpp>
#include <fma/symbol/ConstantNumber.hpp>
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
using namespace FMA::symbol;

#include <iostream>
#include <sstream>
#include <cmath>

// ----------------------------------------------------------------------------
ClassPtr NumberClass::create(const RootModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("Number", "Number"));
  klass->extends(ClassObject);

  klass->setMember("@FMA_TYPE_ID", TypePtr(new InternalNumberValue(serialize::TYPE_NUMBER)));

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("ceil", TypePtr(new InternalFunctionValue("ceil", NumberClass::ceil)));
  proto->setMember("even?", TypePtr(new InternalFunctionValue("even?", NumberClass::even_qm)));
  proto->setMember("floor", TypePtr(new InternalFunctionValue("floor", NumberClass::floor)));
  proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", NumberClass::initialize)));
  proto->setMember("odd?", TypePtr(new InternalFunctionValue("odd?", NumberClass::odd_qm)));
  proto->setMember("times", TypePtr(new InternalFunctionValue("times", NumberClass::times)));
  proto->setMember("to_b", TypePtr(new InternalFunctionValue("to_b", NumberClass::to_b)));
  proto->setMember("to_n", TypePtr(new InternalFunctionValue("to_n", NumberClass::to_n)));
  proto->setMember("to_s", TypePtr(new InternalFunctionValue("to_s", NumberClass::to_s)));
  proto->setMember("to_sym", TypePtr(new InternalFunctionValue("to_sym", NumberClass::to_sym)));

  proto->setMember("==", TypePtr(new InternalFunctionValue("==", NumberClass::op_eq)));
  proto->setMember("<", TypePtr(new InternalFunctionValue("<", NumberClass::op_lt)));
  proto->setMember(">", TypePtr(new InternalFunctionValue(">", NumberClass::op_gt)));

  proto->setMember("+", TypePtr(new InternalFunctionValue("+", NumberClass::op_add)));
  proto->setMember("-", TypePtr(new InternalFunctionValue("-", NumberClass::op_sub)));
  proto->setMember("/", TypePtr(new InternalFunctionValue("/", NumberClass::op_div)));
  proto->setMember("*", TypePtr(new InternalFunctionValue("*", NumberClass::op_mul)));
  proto->setMember("%", TypePtr(new InternalFunctionValue("%", NumberClass::op_rem)));
  proto->setMember("&", TypePtr(new InternalFunctionValue("&", NumberClass::op_and)));
  proto->setMember("|", TypePtr(new InternalFunctionValue("|", NumberClass::op_or)));
  proto->setMember("^", TypePtr(new InternalFunctionValue("^", NumberClass::op_xor)));
  proto->setMember("<<", TypePtr(new InternalFunctionValue("<<", NumberClass::op_lshift)));
  proto->setMember(">>", TypePtr(new InternalFunctionValue(">>", NumberClass::op_rshift)));

  root->setMember("Number", klass);
  return klass;
}

// ----------------------------------------------------------------------------
ResultPtr NumberClass::createInstance(const ContextPtr &context, const long double &value) {
  // Resolve a 
  ClassPtr number = context->getRootLevelContext()->resolve("Number")->asClass();
  if (!number) {
    return ResultPtr(new Result());
  }

  GroupedParameterList parameters;
  parameters.push_back(TypePtr(new InternalNumberValue(value)));
  return Result::executed(context, number->createInstance(context, parameters));
}

// ----------------------------------------------------------------------------
ResultPtr NumberClass::initialize(const ContextPtr &context, const GroupedParameterList &parameter) {
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
ResultPtr NumberClass::ceil(const ContextPtr &context, const GroupedParameterList&) {
  return NumberClass::createInstance(context, std::ceil(context->self()->convertToNumber(context)));
}

// ----------------------------------------------------------------------------
ResultPtr NumberClass::floor(const ContextPtr &context, const GroupedParameterList&) {
  return NumberClass::createInstance(context, std::floor(context->self()->convertToNumber(context)));
}

// ----------------------------------------------------------------------------
ResultPtr NumberClass::even_qm(const ContextPtr &context, const GroupedParameterList&) {
  return BooleanClass::createInstance(context, (int64_t)context->self()->convertToNumber(context) % 2 == 0);
}

// ----------------------------------------------------------------------------
ResultPtr NumberClass::odd_qm(const ContextPtr &context, const GroupedParameterList&) {
  return BooleanClass::createInstance(context, (int64_t)context->self()->convertToNumber(context) % 2 == 1);
}

// ----------------------------------------------------------------------------
ResultPtr NumberClass::to_b(const ContextPtr &context, const GroupedParameterList&) {
  return BooleanClass::createInstance(context, !!context->self()->convertToNumber(context));
}

// ----------------------------------------------------------------------------
ResultPtr NumberClass::to_n(const ContextPtr &context, const GroupedParameterList&) {
  return Result::executed(context, context->self());
}

// ----------------------------------------------------------------------------
ResultPtr NumberClass::to_sym(const ContextPtr &context, const GroupedParameterList&) {
  return SymbolReferenceClass::createInstance(context, ReferencePtr(new ConstantNumber(context->self()->convertToNumber(context))));
}

// ----------------------------------------------------------------------------
ResultPtr NumberClass::to_s(const ContextPtr &context, const GroupedParameterList&) {
  long double num = context->self()->convertToNumber(context);
  int64_t intNum = num;
  std::ostringstream os;

  if (intNum == num) {
    os << intNum;
  } else {
    os << num;
  }

  return StringClass::createInstance(context, os.str());
}

// ---------------------------------------------------------------------------
ResultPtr NumberClass::times(const ContextPtr &context, const GroupedParameterList& params) {
  const TypeList &blocks = params.only_blocks();
  if (!blocks.size()) { return to_n(context, params); }

  ResultPtr result(new Result());
  TypePtr block = blocks.front();
  int64_t maxCount = context->self()->convertToNumber(context);
  for (int64_t index=0; index<maxCount; index++) {
    GroupedParameterList yieldParams;
    yieldParams.push_back(Parameter(NumberClass::createInstance(context, index)->get()));
    result = block->call(context, yieldParams);
  }

  return result;
}

// ---------------------------------------------------------------------------=
ResultPtr NumberClass::foreignOperandCall(const ContextPtr &context, const std::string &op, const TypePtr &right) {
  GroupedParameterList params;

  if (right->hasMember("to_sym")) {
    TypePtr l = context->self()->callDirect("to_sym", context, params)->get();
    TypePtr r = right->callDirect("to_sym", context, params)->get();

    params.push_back(r);
    return l->callDirect(op, context, params);
  }

  context->log().error() << "Unsupported operand " << op << " for number calculation: " << right->asString();
  return ResultPtr(new Result());
}

// ---------------------------------------------------------------------------=
ResultPtr NumberClass::op_add(const ContextPtr &context, const GroupedParameterList& params) {
  const TypeList &args = params.only_args();
  if (!args.size()) { return to_n(context, params); }

  TypePtr right = args.front();
  if (right->isObjectOfType("String")) {
    return StringClass::createInstance(context, context->self()->convertToString(context) + right->convertToString(context));
  }

  if (!right->hasMember("to_n")) {
    return foreignOperandCall(context, "+", right);
  }

  return NumberClass::createInstance(context, context->self()->convertToNumber(context) + right->convertToNumber(context));
}

// ----------------------------------------------------------------------------
ResultPtr NumberClass::op_sub(const ContextPtr &context, const GroupedParameterList& params) {
  const TypeList &args = params.only_args();
  if (!args.size()) { return to_n(context, params); }

  TypePtr right = args.front();
  if (!right->hasMember("to_n")) {
    return foreignOperandCall(context, "-", right);
  }

  return NumberClass::createInstance(context, context->self()->convertToNumber(context) - right->convertToNumber(context));
}

// ----------------------------------------------------------------------------
ResultPtr NumberClass::op_eq(const ContextPtr &context, const GroupedParameterList& params) {
  const TypeList &args = params.only_args();
  if (!args.size()) { return to_n(context, params); }

  TypePtr right = args.front();
  if (!right->hasMember("to_n")) {
    return foreignOperandCall(context, "==", right);
  }

  return BooleanClass::createInstance(context, context->self()->convertToNumber(context) == right->convertToNumber(context));
}

// ----------------------------------------------------------------------------
ResultPtr NumberClass::op_lt(const ContextPtr &context, const GroupedParameterList& params) {
  const TypeList &args = params.only_args();
  if (!args.size()) { return to_n(context, params); }

  TypePtr right = args.front();
  if (!right->hasMember("to_n")) {
    return foreignOperandCall(context, "<", right);
  }

  return BooleanClass::createInstance(context, context->self()->convertToNumber(context) < right->convertToNumber(context));
}

// ----------------------------------------------------------------------------
ResultPtr NumberClass::op_gt(const ContextPtr &context, const GroupedParameterList& params) {
  const TypeList &args = params.only_args();
  if (!args.size()) { return to_n(context, params); }

  TypePtr right = args.front();
  if (!right->hasMember("to_n")) {
    return foreignOperandCall(context, ">", right);
  }

  return BooleanClass::createInstance(context, context->self()->convertToNumber(context) > right->convertToNumber(context));
}

// ----------------------------------------------------------------------------
ResultPtr NumberClass::op_div(const ContextPtr &context, const GroupedParameterList& params) {
  const TypeList &args = params.only_args();
  if (!args.size()) { return to_n(context, params); }

  TypePtr right = args.front();
  if (!right->hasMember("to_n")) {
    return foreignOperandCall(context, "/", right);
  }

  return NumberClass::createInstance(context, context->self()->convertToNumber(context) / right->convertToNumber(context));
}

// ----------------------------------------------------------------------------
ResultPtr NumberClass::op_mul(const ContextPtr &context, const GroupedParameterList& params) {
  const TypeList &args = params.only_args();
  if (!args.size()) { return to_n(context, params); }

  TypePtr right = args.front();
  if (!right->hasMember("to_n")) {
    return foreignOperandCall(context, "*", right);
  }

  return NumberClass::createInstance(context, context->self()->convertToNumber(context) * right->convertToNumber(context));
}

// ----------------------------------------------------------------------------
ResultPtr NumberClass::op_rem(const ContextPtr &context, const GroupedParameterList& params) {
  const TypeList &args = params.only_args();
  if (!args.size()) { return to_n(context, params); }

  TypePtr right = args.front();
  if (!right->hasMember("to_n")) {
    return foreignOperandCall(context, "%", right);
  }

  return NumberClass::createInstance(context, std::fmod(context->self()->convertToNumber(context), right->convertToNumber(context)));
}

// ----------------------------------------------------------------------------
ResultPtr NumberClass::op_and(const ContextPtr &context, const GroupedParameterList& params) {
  const TypeList &args = params.only_args();
  if (!args.size()) { return to_n(context, params); }

  TypePtr right = args.front();
  if (!right->hasMember("to_n")) {
    return foreignOperandCall(context, "&", right);
  }

  return NumberClass::createInstance(context, (uint64_t)context->self()->convertToNumber(context) & (uint64_t)right->convertToNumber(context));
}


// ----------------------------------------------------------------------------
ResultPtr NumberClass::op_or(const ContextPtr &context, const GroupedParameterList& params) {
  const TypeList &args = params.only_args();
  if (!args.size()) { return to_n(context, params); }

  TypePtr right = args.front();
  if (!right->hasMember("to_n")) {
    return foreignOperandCall(context, "|", right);
  }

  return NumberClass::createInstance(context, (uint64_t)context->self()->convertToNumber(context) | (uint64_t)right->convertToNumber(context));
}


// ----------------------------------------------------------------------------
ResultPtr NumberClass::op_xor(const ContextPtr &context, const GroupedParameterList& params) {
  const TypeList &args = params.only_args();
  if (!args.size()) { return to_n(context, params); }

  TypePtr right = args.front();
  if (!right->hasMember("to_n")) {
    return foreignOperandCall(context, "^", right);
  }

  return NumberClass::createInstance(context, (uint64_t)context->self()->convertToNumber(context) ^ (uint64_t)right->convertToNumber(context));
}


// ----------------------------------------------------------------------------
ResultPtr NumberClass::op_lshift(const ContextPtr &context, const GroupedParameterList& params) {
  const TypeList &args = params.only_args();
  if (!args.size()) { return to_n(context, params); }

  TypePtr right = args.front();
  if (!right->hasMember("to_n")) {
    return foreignOperandCall(context, "<<", right);
  }

  return NumberClass::createInstance(context, (uint64_t)context->self()->convertToNumber(context) << (uint64_t)right->convertToNumber(context));
}


// ----------------------------------------------------------------------------
ResultPtr NumberClass::op_rshift(const ContextPtr &context, const GroupedParameterList& params) {
  const TypeList &args = params.only_args();
  if (!args.size()) { return to_n(context, params); }

  TypePtr right = args.front();
  if (!right->hasMember("to_n")) {
    return foreignOperandCall(context, ">>", right);
  }

  return NumberClass::createInstance(context, (uint64_t)context->self()->convertToNumber(context) >> (uint64_t)right->convertToNumber(context));
}

// ----------------------------------------------------------------------------
void NumberClass::serializeObject(const TypePtr &type, const serialize::SerializeObjectPtr &object) {
  TypePtr number = type->getMember("__value");
  long double value = 0;

  if (number->isInternalObjectOfType("Number")) {
    InternalNumberValue *numberValue = dynamic_cast<InternalNumberValue*>(number.get());
    value = numberValue->getValue();
  }

  object->write(&value, sizeof(value));
}

// ----------------------------------------------------------------------------
bool NumberClass::deserializeObject(const serialize::DeserializeObjectPtr &object) {
  object->allowBufferReadWithoutObject();

  long double value;
  if (object->read(&value, sizeof(value)) < sizeof(value)) {
    return false;
  }

  object->store(NumberClass::createInstance(object->getContext(), value)->get());
  return true;
}
