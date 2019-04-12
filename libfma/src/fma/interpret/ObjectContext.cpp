#include <fma/Project.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/interpret/Interpreter.hpp>
#include <fma/interpret/ObjectContext.hpp>
#include <fma/interpret/Result.hpp>

using namespace FMA;
using namespace FMA::types;
using namespace FMA::interpret;

#include <iostream>
#include <sstream>

// ----------------------------------------------------------------------------
ObjectContext::ObjectContext(Interpreter *interpreter, const TypePtr &object)
  : BaseContext(interpreter)
  , object(object)
{
}

// ----------------------------------------------------------------------------
TypePtr ObjectContext::getDeclareLevelObject() {
  return object;
}

// ----------------------------------------------------------------------------
ResultPtr ObjectContext::getMember(const std::string &name) {
  /*if (name == "self") {
    return ResultPtr(new Result(getPointer(), object));
  }*/

  ResultPtr result(new Result(getPointer(), object->getMember(name)));
  result->makeAssignable(getPointer(), name);
  return result;
}

// ----------------------------------------------------------------------------
ResultPtr ObjectContext::setMember(const std::string &name, const TypePtr &value) {
  object->setMember(name, value);

  ResultPtr result(new Result(getPointer(), value));
  result->makeAssignable(getPointer(), name);
  return result;
}

// ----------------------------------------------------------------------------
ResultPtr ObjectContext::resolve(const std::string &name) {
  ResultPtr member = getMember(name);
  if (!member->isUndefined()) {
    return member;
  }

  ResultPtr base = BaseContext::resolve(name);
  if (!base->isUndefined()) {
    return base;
  }

  return member;
}

// ----------------------------------------------------------------------------
TypePtr ObjectContext::self() {
  return object;
}

// ----------------------------------------------------------------------------
std::string ObjectContext::asString() const {
  std::ostringstream os;
  os << "<ObjectContext " << object->asString() << ">";
  return os.str();
}

// ----------------------------------------------------------------------------
