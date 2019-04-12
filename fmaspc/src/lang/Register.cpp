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

using namespace FMASPC::lang;
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
