#include <fma/Project.hpp>
#include <fma/types/Class.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/interpret/Interpreter.hpp>
#include <fma/interpret/ClassContext.hpp>
#include <fma/interpret/ObjectContext.hpp>
#include <fma/interpret/Result.hpp>

using namespace FMA;
using namespace FMA::types;
using namespace FMA::interpret;

#include <iostream>
#include <sstream>

// ----------------------------------------------------------------------------
ClassContext::ClassContext(Interpreter *interpreter, const ClassPtr &klass)
  : BaseContext(interpreter)
  , klass(klass)
{
}

// ----------------------------------------------------------------------------
TypePtr ClassContext::getDeclareLevelObject() {
  return klass;
}

// ----------------------------------------------------------------------------
ContextPtr ClassContext::getDeclareLevelContext() {
  return ContextPtr(new ObjectContext(interpreter, klass));
}

// ----------------------------------------------------------------------------
ContextPtr ClassContext::getMacroDeclareLevelContext() {
  return ContextPtr(new ObjectContext(interpreter, klass->getPrototype()));
}

// ----------------------------------------------------------------------------
ResultPtr ClassContext::getMember(const std::string &name) {
  ResultPtr result(new Result(getPointer(), klass->getPrototypeMember(name)));
  result->makeAssignable(getPointer(), name);
  return result;
}

// ----------------------------------------------------------------------------
ResultPtr ClassContext::setMember(const std::string &name, const TypePtr &value) {
  klass->setMember(name, value);

  ResultPtr result(new Result(getPointer(), value));
  result->makeAssignable(getPointer(), name);
  return result;
}

// ----------------------------------------------------------------------------
ResultPtr ClassContext::resolve(const std::string &name) {
  ResultPtr result(new Result(getPointer(), klass->getMember(name)));
  result->makeAssignable(getPointer(), name);
  return result;
}

// ----------------------------------------------------------------------------
std::string ClassContext::asString() const {
  std::ostringstream os;
  os << "<ClassContext " << klass->asString() << ">";
  return os.str();
}

// ----------------------------------------------------------------------------
