#include <fma/core/Class.hpp>
#include <fma/core/Boolean.hpp>
#include <fma/core/ImmediateNumber.hpp>
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
ClassPtr ImmediateNumberClass::create(const RootModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("ImmediateNumber", "ImmediateNumber"));
  klass->extends(ClassObject);

  klass->setMember("@FMA_TYPE_ID", TypePtr(new InternalNumberValue(serialize::TYPE_IMMEDIATENUMBER)));

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", ImmediateNumberClass::initialize)));

  root->setMember("ImmediateNumber", klass);
  return klass;
}

// ----------------------------------------------------------------------------
ResultPtr ImmediateNumberClass::createInstance(const ContextPtr &context, const TypePtr &value) {
  // Resolve a 
  ClassPtr number = context->getRootLevelContext()->resolve("ImmediateNumber")->asClass();
  if (!number) {
    return ResultPtr(new Result());
  }

  GroupedParameterList parameters;
  parameters.push_back(value);
  return Result::executed(context, number->createInstance(context, parameters));
}


// ----------------------------------------------------------------------------
ResultPtr ImmediateNumberClass::initialize(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    return ResultPtr(new Result());
  }
  
  context->self()->setMember("number", args.front());

  return Result::executed(context, context->self());
}
