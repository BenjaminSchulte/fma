#include <fma/Project.hpp>
#include <fma/types/Module.hpp>
#include <fma/interpret/Interpreter.hpp>
#include <fma/interpret/ModuleContext.hpp>
#include <fma/interpret/Result.hpp>

using namespace FMA;
using namespace FMA::types;
using namespace FMA::interpret;

#include <iostream>
#include <sstream>

// ----------------------------------------------------------------------------
ModuleContext::ModuleContext(Interpreter *interpreter, const ModulePtr &module)
  : BaseContext(interpreter)
  , module(module)
{
}

// ----------------------------------------------------------------------------
TypePtr ModuleContext::getDeclareLevelObject() {
  return module;
}

// ----------------------------------------------------------------------------
ResultPtr ModuleContext::getMember(const std::string &name) {
  ResultPtr result(new Result(getPointer(), module->getMember(name)));
  result->makeAssignable(getPointer(), name);
  return result;
}

// ----------------------------------------------------------------------------
ResultPtr ModuleContext::setMember(const std::string &name, const TypePtr &value) {
  module->setMember(name, value);

  ResultPtr result(new Result(getPointer(), value));
  result->makeAssignable(getPointer(), name);
  return result;
}

// ----------------------------------------------------------------------------
ResultPtr ModuleContext::resolve(const std::string &name) {
  return getMember(name);
}

// ----------------------------------------------------------------------------
std::string ModuleContext::asString() const {
  std::ostringstream os;
  os << "<ModuleContext " << module->asString() << ">";
  return os.str();
}

// ----------------------------------------------------------------------------
std::string ModuleContext::getNameHint() const {
  return module->getName();
}

// ----------------------------------------------------------------------------
