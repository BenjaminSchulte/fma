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

#include <superfx/Register.hpp>

using namespace FMA::superfx;
using namespace FMA;
using namespace FMA::core;
using namespace FMA::types;
using namespace FMA::interpret;
#include <iostream>
#include <sstream>

// ----------------------------------------------------------------------------
ClassPtr RegisterClass::create(const RootModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("SuperFxRegister", "SuperFxRegister"));
  klass->extends(ClassObject);

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", RegisterClass::initialize)));
  proto->setMember("size", TypePtr(new InternalFunctionValue("size", RegisterClass::size)));
  proto->setMember("pretend_size", TypePtr(new InternalFunctionValue("pretend_size", RegisterClass::pretend_size)));
  proto->setMember("is_a!", TypePtr(new InternalFunctionValue("is_a!", RegisterClass::is_a)));
  proto->setMember("==", TypePtr(new InternalFunctionValue("==", RegisterClass::op_equal)));

  root->setMember("SuperFxRegister", klass);
  return klass;
}

// ----------------------------------------------------------------------------
ResultPtr RegisterClass::createInstance(const ContextPtr &context, const std::string &value, int index) {
  // Resolve a 
  ClassPtr string = context->getRootLevelContext()->resolve("SuperFxRegister")->asClass();
  if (!string) {
    return ResultPtr(new Result());
  }

  GroupedParameterList parameters;
  parameters.push_back(StringClass::createInstance(context, value)->get());
  parameters.push_back(NumberClass::createInstance(context, index)->get());
  return Result::executed(context, string->createInstance(context, parameters));
}

// ----------------------------------------------------------------------------
ResultPtr RegisterClass::initialize(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  
  switch (args.size()) {
    case 2:
      context->self()->setMember("name", args.front());
      context->self()->setMember("index", args.back());
      break;
    
    default:
      context->log().error() << "Missing arguments for SuperFxRegister";
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
ResultPtr RegisterClass::op_equal(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    return ResultPtr(new Result());
  }

  const TypePtr &other = args.front();
  if (!other->isObjectOfType("SuperFxRegister")) {
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
