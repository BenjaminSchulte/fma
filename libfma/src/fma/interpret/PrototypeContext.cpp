#include <fma/Project.hpp>
#include <fma/types/Class.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/interpret/Interpreter.hpp>
#include <fma/interpret/PrototypeContext.hpp>
#include <fma/interpret/ObjectContext.hpp>
#include <fma/interpret/Result.hpp>

using namespace FMA;
using namespace FMA::types;
using namespace FMA::interpret;

#include <iostream>
#include <sstream>

// ----------------------------------------------------------------------------
PrototypeContext::PrototypeContext(Interpreter *interpreter, const ClassPtr &klass)
  : BaseContext(interpreter)
  , klass(klass)
{
}

// ----------------------------------------------------------------------------
TypePtr PrototypeContext::getDeclareLevelObject() {
  return klass;
}

// ----------------------------------------------------------------------------
ContextPtr PrototypeContext::getDeclareLevelContext() {
  return ContextPtr(new ObjectContext(interpreter, klass));
}

// ----------------------------------------------------------------------------
ContextPtr PrototypeContext::getMacroDeclareLevelContext() {
  return ContextPtr(new ObjectContext(interpreter, klass->getPrototype()));
}

// ----------------------------------------------------------------------------
ResultPtr PrototypeContext::getMember(const std::string &name) {
  ResultPtr result(new Result(getPointer(), klass->getPrototypeMember(name)));
  result->makeAssignable(getPointer(), name);
  return result;
}

// ----------------------------------------------------------------------------
ResultPtr PrototypeContext::setMember(const std::string &name, const TypePtr &value) {
  klass->getPrototype()->setMember(name, value);

  ResultPtr result(new Result(getPointer(), value));
  result->makeAssignable(getPointer(), name);
  return result;
}

// ----------------------------------------------------------------------------
ResultPtr PrototypeContext::resolve(const std::string &name) {
  ResultPtr result(new Result(getPointer(), klass->getPrototypeMember(name)));
  result->makeAssignable(getPointer(), name);
  return result;
}

// ----------------------------------------------------------------------------
std::string PrototypeContext::asString() const {
  std::ostringstream os;
  os << "<PrototypeContext " << klass->asString() << ">";
  return os.str();
}

// ----------------------------------------------------------------------------
