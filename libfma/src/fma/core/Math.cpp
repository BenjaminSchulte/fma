#include <boost/filesystem.hpp>
#include <fma/core/Class.hpp>
#include <fma/core/Math.hpp>
#include <fma/core/String.hpp>
#include <fma/core/Number.hpp>
#include <fma/core/SymbolReference.hpp>
#include <fma/core/DataBlock.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/Parameter.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/interpret/Interpreter.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/InstanceContext.hpp>
#include <fma/plugin/MemoryPluginAdapter.hpp>
#include <fma/Project.hpp>
#include <math.h>

using namespace FMA;
using namespace FMA::core;
using namespace FMA::types;
using namespace FMA::interpret;
using namespace FMA::plugin;

#include <iostream>
#include <sstream>

// ----------------------------------------------------------------------------
ClassPtr MathClass::create(const RootModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("Math", "Math"));
  klass->extends(ClassObject);

  klass->setMember("PI", TypePtr(new InternalFunctionValue("PI", MathClass::PI)));
  klass->setMember("sin", TypePtr(new InternalFunctionValue("sin", MathClass::sin)));
  klass->setMember("cos", TypePtr(new InternalFunctionValue("cos", MathClass::cos)));

  root->setMember("Math", klass);
  return klass;
}

// ----------------------------------------------------------------------------
ResultPtr MathClass::PI(const ContextPtr &context, const GroupedParameterList &) {
  return NumberClass::createInstance(context, M_PI);
}

// ----------------------------------------------------------------------------
ResultPtr MathClass::sin(const ContextPtr &context, const GroupedParameterList &parameter) {
  auto &param = parameter.only_args();
  if (param.empty()) { return NumberClass::createInstance(context, 0); }
  return NumberClass::createInstance(context, ::sin(param.front()->convertToNumber(context)));
}

// ----------------------------------------------------------------------------
ResultPtr MathClass::cos(const ContextPtr &context, const GroupedParameterList &parameter) {
  auto &param = parameter.only_args();
  if (param.empty()) { return NumberClass::createInstance(context, 0); }
  return NumberClass::createInstance(context, ::cos(param.front()->convertToNumber(context)));
}
