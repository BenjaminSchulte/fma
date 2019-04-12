#include <fma/interpret/Result.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/interpret/ObjectContext.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Interpreter.hpp>
#include <fma/types/Base.hpp>
#include <fma/types/Class.hpp>
#include <fma/types/Object.hpp>
#include <fma/types/Undefined.hpp>
#include <fma/core/String.hpp>
#include <fma/Project.hpp>
#include <iostream>

using namespace FMA;
using namespace FMA::interpret;
using namespace FMA::types;
using namespace FMA::core;

// ----------------------------------------------------------------------------
Result::Result()
  : value(undefinedValue)
  , type(STANDARD)
  , isExecuted(true)
  , executeContext(NULL)
{
}

// ----------------------------------------------------------------------------
Result::Result(const ContextPtr &context, const TypePtr &value) 
  : value(value)
  , contextObject()
  , type(STANDARD)
  , isExecuted(false)
  , executeContext(context)
{
}

// ----------------------------------------------------------------------------
Result::Result(const ContextPtr &context, const TypePtr &value, const TypePtr &contextObject)
  : value(value)
  , contextObject(contextObject)
  , type(STANDARD)
  , isExecuted(false)
  , executeContext(context)
{
}

// ----------------------------------------------------------------------------
ResultPtr Result::executed(const ContextPtr &context, const types::TypePtr &value) {
  ResultPtr result = ResultPtr(new Result(context, value));
  result->pretendExecuted();
  return result;
}

// ----------------------------------------------------------------------------
ResultPtr Result::returns(const types::TypePtr &value) {
  ResultPtr result = ResultPtr(new Result(NULL, value));
  result->type = Result::EARLY_RETURN;
  return result;
}

// ----------------------------------------------------------------------------
void Result::pretendExecuted() {
  isExecuted = true;
}

// ----------------------------------------------------------------------------
void Result::requireExecuted() {
  if (isExecuted) {
    return;
  }

  GroupedParameterList parameter;
  ContextPtr callContext;
  
  if (hasContextObject()) {
    callContext = ContextPtr(new ObjectContext(executeContext->getInterpreter(), getContextObject()));
  } else {
    callContext = ContextPtr(new BaseContext(executeContext->getInterpreter()));
  }

  ResultPtr result;
  if (value->isUndefined() && executeContext) {
    if (!assignName.empty()) {
      executeContext->log().error() << location << "Expression result is <undefined>";
    } else {
      executeContext->log().error() << location << "Expression result is <undefined> for " << assignName;
    }

    result = ResultPtr(new Result());
  } else {
    auto ref = executeContext->getInterpreter()->stack().enter(location);
    result = value->call(callContext, parameter);
  }

  result->pretendExecuted();
  value = result->get();
  executeContext = NULL;
  isExecuted = true;
}

// ----------------------------------------------------------------------------
const TypePtr &Result::get() {
  requireExecuted();

  return value;
}

// ----------------------------------------------------------------------------
bool Result::isUndefined() {
  if (value->isUndefined()) {
    return true;
  }

  return false;
}

// ----------------------------------------------------------------------------
bool Result::isClass() {
  requireExecuted();

  return value->isClass();
}

// ----------------------------------------------------------------------------
bool Result::isMacro() {
  return value->isMacro();
}

// ----------------------------------------------------------------------------
bool Result::isObject() {
  requireExecuted();

  return value->isObject();
}

// ----------------------------------------------------------------------------
bool Result::isModule() {
  requireExecuted();

  return value->isModule();
}

// ----------------------------------------------------------------------------
ClassPtr Result::asClass() {
  requireExecuted();

  if (!isClass()) {
    return ClassPtr();
  }

  return std::dynamic_pointer_cast<Class>(value);
}

// ----------------------------------------------------------------------------
ObjectPtr Result::asObject() {
  requireExecuted();

  if (!isObject()) {
    return ObjectPtr();
  }

  return std::dynamic_pointer_cast<Object>(value);
}

// ----------------------------------------------------------------------------
ObjectPtr Result::asObjectOfType(const std::string &name) {
  requireExecuted();

  ObjectPtr object = asObject();
  if (!object) {
    return ObjectPtr();
  }

  if (object->getClass()->getName() != name) {
    return ObjectPtr();
  }

  return object;
}

// ----------------------------------------------------------------------------
void Result::assign(const TypePtr &newValue) {
  if (!isAssignable()) {
    return;
  }

  pretendExecuted();
  value = newValue;

  ResultPtr assign = ownerContext->getMember("[]=");
  if (assign->isUndefined()) {
    ownerContext->setMember(assignName, value);
  } else {
    GroupedParameterList params;
    params.push_back(Parameter(StringClass::createInstance(ownerContext, assignName)->get()));
    params.push_back(Parameter(newValue));
    assign->pretendExecuted();
    assign->get()->call(ownerContext, params);
  }
}

// ----------------------------------------------------------------------------
void Result::dump() const {
  if (!value) {
    std::cout << "<Invalid>" << std::endl;
    return;
  }

  return value->dump("");
}

// ----------------------------------------------------------------------------
std::string Result::asString() const {
  if (!value) {
    return "<Invalid>";
  }

  return value->asString();
}

// ----------------------------------------------------------------------------
