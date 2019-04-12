#include <fma/core/Class.hpp>
#include <fma/core/Boolean.hpp>
#include <fma/core/Range.hpp>
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
#include <cmath>

// ----------------------------------------------------------------------------
ClassPtr RangeClass::create(const RootModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("Range", "Range"));
  klass->extends(ClassObject);

  klass->setMember("@FMA_TYPE_ID", TypePtr(new InternalNumberValue(serialize::TYPE_RANGE)));

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("first", TypePtr(new InternalFunctionValue("first", RangeClass::first)));
  proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", RangeClass::initialize)));
  proto->setMember("last", TypePtr(new InternalFunctionValue("last", RangeClass::last)));
  proto->setMember("to_s", TypePtr(new InternalFunctionValue("to_s", RangeClass::to_s)));

  root->setMember("Range", klass);
  return klass;
}

// ----------------------------------------------------------------------------
ResultPtr RangeClass::createInstance(const ContextPtr &context, const int64_t &first, const int64_t &last) {
  // Resolve a 
  ClassPtr number = context->getRootLevelContext()->resolve("Range")->asClass();
  if (!number) {
    return ResultPtr(new Result());
  }

  GroupedParameterList parameters;
  parameters.push_back(TypePtr(new InternalNumberValue(first)));
  parameters.push_back(TypePtr(new InternalNumberValue(last)));
  return Result::executed(context, number->createInstance(context, parameters));
}

// ----------------------------------------------------------------------------
ResultPtr RangeClass::initialize(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  
  switch (args.size()) {
    case 0:
    case 1:
      context->self()->setMember("__first", TypePtr(new InternalNumberValue(0)));
      context->self()->setMember("__last", TypePtr(new InternalNumberValue(0)));
      break;
    
    case 2:
    default:
      context->self()->setMember("__first", args.front());
      context->self()->setMember("__last", args.back());
      break;
  }

  return Result::executed(context, context->self());
}

// ----------------------------------------------------------------------------
ResultPtr RangeClass::first(const ContextPtr &context, const GroupedParameterList &) {
  return NumberClass::createInstance(context, context->self()->convertToRange(context).first);
}

// ----------------------------------------------------------------------------
ResultPtr RangeClass::last(const ContextPtr &context, const GroupedParameterList &) {
  return NumberClass::createInstance(context, context->self()->convertToRange(context).last);
}

// ----------------------------------------------------------------------------
ResultPtr RangeClass::to_s(const ContextPtr &context, const GroupedParameterList &) {
  const auto &range = context->self()->convertToRange(context);

  std::ostringstream os;
  os << range.first << ".." << range.last;

  return StringClass::createInstance(context, os.str());
}