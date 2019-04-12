#include <fma/Project.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/interpret/Interpreter.hpp>
#include <fma/interpret/BlockContext.hpp>
#include <fma/interpret/Result.hpp>

using namespace FMA;
using namespace FMA::types;
using namespace FMA::interpret;

#include <iostream>
#include <sstream>

// ----------------------------------------------------------------------------
BlockContext::BlockContext(Interpreter *interpreter, const TypePtr &object)
  : BaseContext(interpreter)
  , object(object)
{
}

// ----------------------------------------------------------------------------
TypePtr BlockContext::getDeclareLevelObject() {
  return object;
}

// ----------------------------------------------------------------------------
ResultPtr BlockContext::getMember(const std::string &name) {
  ResultPtr result(new Result(getPointer(), object->getMember(name)));
  result->makeAssignable(getPointer(), name);
  return result;
}

// ----------------------------------------------------------------------------
ResultPtr BlockContext::setMember(const std::string &name, const TypePtr &value) {
  object->setMember(name, value);

  ResultPtr result(new Result(getPointer(), value));
  result->makeAssignable(getPointer(), name);
  return result;
}

// ----------------------------------------------------------------------------
std::string BlockContext::asString() const {
  std::ostringstream os;
  os << "<BlockContext " << object->asString() << ">";
  return os.str();
}

// ----------------------------------------------------------------------------
