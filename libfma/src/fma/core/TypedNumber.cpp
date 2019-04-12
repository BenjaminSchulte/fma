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

  root->setMember("TypedNumber", klass);
  return klass;
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
