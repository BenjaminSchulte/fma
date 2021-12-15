#include <fma/core/Class.hpp>
#include <fma/core/String.hpp>
#include <fma/core/Array.hpp>
#include <fma/core/Number.hpp>
#include <fma/core/TypedNumber.hpp>
#include <fma/core/Boolean.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/types/Object.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/InstanceContext.hpp>
#include <fma/interpret/Interpreter.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/Parameter.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/Project.hpp>

#include <unicode/unistr.h>

#include <boost/locale.hpp>

#include <lang/Register.hpp>

using namespace FMA65816::lang;
using namespace FMA;
using namespace FMA::core;
using namespace FMA::types;
using namespace FMA::interpret;
#include <iostream>
#include <sstream>

// ----------------------------------------------------------------------------
ClassPtr RegisterClass::create(const RootModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("Register", "Register"));
  klass->extends(ClassObject);

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", RegisterClass::initialize)));
  proto->setMember("size", TypePtr(new InternalFunctionValue("size", RegisterClass::size)));
  proto->setMember("pretend_size", TypePtr(new InternalFunctionValue("pretend_size", RegisterClass::pretend_size)));
  proto->setMember("is_a!", TypePtr(new InternalFunctionValue("is_a!", RegisterClass::is_a)));
  proto->setMember("[]", TypePtr(new InternalFunctionValue("[]", RegisterClass::op_index)));
  proto->setMember("==", TypePtr(new InternalFunctionValue("==", RegisterClass::op_equal)));
  proto->setMember(".", TypePtr(new InternalFunctionValue(".", RegisterClass::op_member)));

  root->setMember("Register", klass);
  return klass;
}

// ----------------------------------------------------------------------------
ResultPtr RegisterClass::createInstance(const ContextPtr &context, const std::string &value) {
  // Resolve a 
  ClassPtr string = context->getRootLevelContext()->resolve("Register")->asClass();
  if (!string) {
    return ResultPtr(new Result());
  }

  GroupedParameterList parameters;
  parameters.push_back(StringClass::createInstance(context, value)->get());
  return Result::executed(context, string->createInstance(context, parameters));
}

// ----------------------------------------------------------------------------
ResultPtr RegisterClass::initialize(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  
  switch (args.size()) {
    case 0:
      context->self()->setMember("name", TypePtr(new InternalStringValue("")));
      break;
    
    case 1:
    default:
      context->self()->setMember("name", args.front());
      break;
  }

  return Result::executed(context, context->self());
}

// ----------------------------------------------------------------------------
ResultPtr RegisterClass::is_a(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    return ResultPtr(new Result());
  }

  context->self()->setMember("__register_type", args.front());

  return Result::executed(context, context->self());
}

// ----------------------------------------------------------------------------
ResultPtr RegisterClass::op_index(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    return ResultPtr(new Result());
  }

  TypePtr typeHint = context->self()->getMember("__register_type");
  if (typeHint->isUndefined()) {
    context->log().error() << "Unable to access member offset, because no type has been defined";
    return ResultPtr(new Result());
  }
  
  GroupedParameterList callParam;
  callParam.push_back(args.front());

  uint64_t index = typeHint->callDirect("offset_of", context, callParam)->get()->convertToNumber(context);

  TypePtr indexInstance = NumberClass::createInstance(context, index)->get();
  std::string name = context->self()->getMember("name")->convertToString(context);

  return TypedNumberClass::createInstance(context, indexInstance, "register_member_" + name);
}

// ----------------------------------------------------------------------------
ResultPtr RegisterClass::op_member(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    return ResultPtr(new Result());
  }

  TypePtr typeHint = context->self()->getMember("__register_type");
  if (typeHint->isUndefined()) {
    return ResultPtr(new Result());
  }

  std::string memberName = args.front()->convertToString(context);
  return ResultPtr(new Result(context, typeHint->getMember(memberName)));
}

// ----------------------------------------------------------------------------
ResultPtr RegisterClass::op_equal(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    return ResultPtr(new Result());
  }

  const TypePtr &other = args.front();
  if (!other->isObjectOfType("Register")) {
    return ResultPtr(BooleanClass::createInstance(context, false));
  }

  bool result = other->getMember("name")->convertToString(context) == context->self()->getMember("name")->convertToString(context);

  return ResultPtr(BooleanClass::createInstance(context, result));
}

// ----------------------------------------------------------------------------
ResultPtr RegisterClass::size(const ContextPtr &context, const GroupedParameterList &) {
  ContextPtr global = context->getInterpreter()->getGlobalContext();
  if (global) {
    ResultPtr block = global->resolve("::__current_block");
    if (!block->isUndefined()) {

      TypePtr set = block->get()->getMember("__get_register_size");
      ContextPtr callContext(new InstanceContext(context->getInterpreter(), block->get()->asObject(), "__get_register_size"));

      GroupedParameterList parameters;
      parameters.push_back(context->self()->getMember("name"));
      return set->call(callContext, parameters);
    }
  }

  return Result::executed(context, context->self());
}

// ----------------------------------------------------------------------------
ResultPtr RegisterClass::pretend_size(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    return ResultPtr(new Result());
  }

  ContextPtr global = context->getInterpreter()->getGlobalContext();
  if (global) {
    ResultPtr block = global->resolve("::__current_block");
    if (!block->isUndefined()) {

      TypePtr set = block->get()->getMember("__set_register_size");
      ContextPtr callContext(new InstanceContext(context->getInterpreter(), block->get()->asObject(), "__set_register_size"));

      GroupedParameterList parameters;
      parameters.push_back(context->self()->getMember("name"));
      parameters.push_back(args.front());
      set->call(callContext, parameters);
    }
  }

  return Result::executed(context, context->self());
}
