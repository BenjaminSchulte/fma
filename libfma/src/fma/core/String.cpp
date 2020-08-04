#include <fma/core/Class.hpp>
#include <fma/core/String.hpp>
#include <fma/core/Array.hpp>
#include <fma/core/Number.hpp>
#include <fma/core/Boolean.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/types/Object.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/Parameter.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/serialize/SerializeTypes.hpp>
#include <fma/Project.hpp>
#include <fma/util/sha256.hpp>

#include <unicode/unistr.h>

#include <boost/locale.hpp>

using namespace FMA;
using namespace FMA::core;
using namespace FMA::types;
using namespace FMA::interpret;
#include <iostream>
#include <sstream>

// ----------------------------------------------------------------------------
ClassPtr StringClass::create(const RootModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("String", "String"));
  klass->extends(ClassObject);

  klass->setMember("@FMA_TYPE_ID", TypePtr(new InternalNumberValue(serialize::TYPE_STRING)));

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("append", TypePtr(new InternalFunctionValue("append", StringClass::append)));
  proto->setMember("bytes", TypePtr(new InternalFunctionValue("bytes", StringClass::bytes)));
  proto->setMember("downcase", TypePtr(new InternalFunctionValue("downcase", StringClass::downcase)));
  proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", StringClass::initialize)));
  proto->setMember("length", TypePtr(new InternalFunctionValue("length", StringClass::length)));
  proto->setMember("prepend", TypePtr(new InternalFunctionValue("prepend", StringClass::prepend)));
  proto->setMember("to_s", TypePtr(new InternalFunctionValue("to_s", StringClass::to_s)));
  proto->setMember("to_n", TypePtr(new InternalFunctionValue("to_n", StringClass::to_n)));
  proto->setMember("sha256", TypePtr(new InternalFunctionValue("sha256", StringClass::sha256)));
  proto->setMember("upcase", TypePtr(new InternalFunctionValue("upcase", StringClass::upcase)));
  proto->setMember("empty?", TypePtr(new InternalFunctionValue("empty?", StringClass::empty_qm)));

  proto->setMember("==", TypePtr(new InternalFunctionValue("==", StringClass::op_eq)));
  proto->setMember("+", TypePtr(new InternalFunctionValue("+", StringClass::op_add)));
  proto->setMember("*", TypePtr(new InternalFunctionValue("*", StringClass::op_mul)));

  root->setMember("String", klass);
  return klass;
}

// ----------------------------------------------------------------------------
ResultPtr StringClass::createInstance(const ContextPtr &context, const std::string &value) {
  // Resolve a 
  ClassPtr string = context->getRootLevelContext()->resolve("String")->asClass();
  if (!string) {
    return ResultPtr(new Result());
  }

  GroupedParameterList parameters;
  parameters.push_back(TypePtr(new InternalStringValue(value)));
  return Result::executed(context, string->createInstance(context, parameters));
}

// ----------------------------------------------------------------------------
ResultPtr StringClass::initialize(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  
  switch (args.size()) {
    case 0:
      context->self()->setMember("__value", TypePtr(new InternalStringValue("")));
      break;
    
    case 1:
    default:
      context->self()->setMember("__value", args.front());
      break;
  }

  return Result::executed(context, context->self());
}

// ----------------------------------------------------------------------------
ResultPtr StringClass::bytes(const ContextPtr &context, const GroupedParameterList&) {
  TypeList items;

  std::string str = context->self()->convertToString(context);
  icu::UnicodeString us(str.c_str());

  uint64_t length = us.length();
  for (uint64_t index=0; index<length; index++) {
    items.push_back(NumberClass::createInstance(context, us.char32At(index))->get());
  }

  return ArrayClass::createInstance(context, items);
}

// ----------------------------------------------------------------------------
ResultPtr StringClass::length(const ContextPtr &context, const GroupedParameterList&) {
  std::string old = context->self()->convertToString(context);
  icu::UnicodeString us(old.c_str());

  return NumberClass::createInstance(context, us.length());
}

// ----------------------------------------------------------------------------
ResultPtr StringClass::empty_qm(const ContextPtr &context, const GroupedParameterList&) {
  std::string old = context->self()->convertToString(context);

  return BooleanClass::createInstance(context, old.length() == 0);
}

// ----------------------------------------------------------------------------
ResultPtr StringClass::to_n(const ContextPtr &context, const GroupedParameterList &parameter) {
  uint8_t base = 10;
  const TypeList &args = parameter.only_args();
  if (args.size()) {
    base = args.front()->convertToNumber(context);
  }

  std::string old = context->self()->convertToString(context);

  try {
    if (base == 10) {
      return NumberClass::createInstance(context, std::stold(old));
    } else {
      return NumberClass::createInstance(context, std::stol(old, NULL, base));
    }
  } catch (const std::invalid_argument &) {
    context->log().error() << "Could not convert string '" << old << "' to number";
  }

  return NumberClass::createInstance(context, 0);
}

// ----------------------------------------------------------------------------
ResultPtr StringClass::to_s(const ContextPtr &context, const GroupedParameterList&) {
  return Result::executed(context, context->self());
}

// ----------------------------------------------------------------------------
ResultPtr StringClass::append(const ContextPtr &context, const GroupedParameterList& params) {
  std::stringstream os;
  os << context->self()->convertToString(context);

  for (auto &param : params.only_args()) {
    os << param->convertToString(context);
  }

  return StringClass::createInstance(context, os.str());
}

// ----------------------------------------------------------------------------
ResultPtr StringClass::prepend(const ContextPtr &context, const GroupedParameterList& params) {
  std::stringstream os;
  
  for (auto &param : params.only_args()) {
    os << param->convertToString(context);
  }
  os << context->self()->convertToString(context);

  return StringClass::createInstance(context, os.str());
}

// ----------------------------------------------------------------------------
ResultPtr StringClass::downcase(const ContextPtr &context, const GroupedParameterList&) {
  std::string old = context->self()->convertToString(context);;
  std::string upper = boost::locale::to_lower(old);
  return StringClass::createInstance(context, upper);
}

// ----------------------------------------------------------------------------
ResultPtr StringClass::upcase(const ContextPtr &context, const GroupedParameterList&) {
  std::string old = context->self()->convertToString(context);
  std::string upper = boost::locale::to_upper(old);
  return StringClass::createInstance(context, upper);
}

// ----------------------------------------------------------------------------
ResultPtr StringClass::sha256(const ContextPtr &context, const GroupedParameterList&) {
  std::string str = context->self()->convertToString(context);
  FMA::util::SHA256 sha;
  sha(str);
  return StringClass::createInstance(context, sha.getHash());
}

// ----------------------------------------------------------------------------
ResultPtr StringClass::op_eq(const ContextPtr &context, const GroupedParameterList& params) {
  const TypeList &args = params.only_args();
  if (!args.size()) { return to_s(context, params); }

  TypePtr right = args.front();
  if (!right->hasMember("to_s")) {
    context->log().error() << "Unsupported operand for string calculation: " << right->asString();
    return to_s(context, params);
  }

  return BooleanClass::createInstance(context, context->self()->convertToString(context) == right->convertToString(context));
}

// ---------------------------------------------------------------------------
ResultPtr StringClass::op_add(const ContextPtr &context, const GroupedParameterList& params) {
  const TypeList &args = params.only_args();
  if (!args.size()) { return to_s(context, params); }

  TypePtr right = args.front();
  if (!right->hasMember("to_s")) {
    context->log().error() << "Unsupported operand for string calculation: " << right->asString();
    return to_s(context, params);
  }

  return StringClass::createInstance(context, context->self()->convertToString(context) + right->convertToString(context));
}

// ---------------------------------------------------------------------------
ResultPtr StringClass::op_mul(const ContextPtr &context, const GroupedParameterList& params) {
  const TypeList &args = params.only_args();
  if (!args.size()) { return to_s(context, params); }

  TypePtr right = args.front();
  if (!right->hasMember("to_n")) {
    context->log().error() << "Unsupported operand for string calculation: " << right->asString();
    return to_s(context, params);
  }

  std::ostringstream os;
  std::string old = context->self()->convertToString(context);
  for (int64_t times=right->convertToNumber(context); times>0; times--) {
    os << old;
  }

  return StringClass::createInstance(context, os.str());
}

// ----------------------------------------------------------------------------
void StringClass::serializeObject(const TypePtr &type, const serialize::SerializeObjectPtr &object) {
  object->write(type->convertToString(object->getContext()));
}

// ----------------------------------------------------------------------------
bool StringClass::deserializeObject(const serialize::DeserializeObjectPtr &object) {
  object->allowBufferReadWithoutObject();
  object->store(StringClass::createInstance(object->getContext(), object->readString())->get());

  return true;
}


// ----------------------------------------------------------------------------