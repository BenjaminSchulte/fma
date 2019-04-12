#include <fma/Project.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/types/Object.hpp>
#include <fma/types/Class.hpp>
#include <fma/interpret/Interpreter.hpp>
#include <fma/interpret/StaticMemberContext.hpp>
#include <fma/interpret/Result.hpp>

using namespace FMA;
using namespace FMA::types;
using namespace FMA::interpret;

#include <iostream>
#include <sstream>

// ----------------------------------------------------------------------------
StaticMemberContext::StaticMemberContext(Interpreter *interpreter, const ClassPtr &object, const std::string &name)
  : BaseContext(interpreter)
  , name(name)
  , object(object)
{
}

// ----------------------------------------------------------------------------
TypePtr StaticMemberContext::getDeclareLevelObject() {
  return object;
}

// ----------------------------------------------------------------------------
ResultPtr StaticMemberContext::getMember(const std::string &name) {
  if (name == "self") {
    return ResultPtr(new Result(getPointer(), object));
  }

  ResultPtr result(new Result(getPointer(), object->getMember(name)));
  result->makeAssignable(getPointer(), name);
  return result;
}

// ----------------------------------------------------------------------------
ResultPtr StaticMemberContext::setMember(const std::string &name, const TypePtr &value) {
  object->setMember(name, value);

  ResultPtr result(new Result(getPointer(), value));
  result->makeAssignable(getPointer(), name);
  return result;
}

// ----------------------------------------------------------------------------
ResultPtr StaticMemberContext::resolve(const std::string &name) {
  if (name == "super") {
    ClassPtr parent = object->getParent();

    if (parent) {
      TypePtr item = parent->getMember(this->name);
      if (!item->isUndefined()) {
        ResultPtr result(new Result(getPointer(), item, object));
        return result;
      }
    }
  }

  TypePtr member = object->getMember(name);
  if (!member->isUndefined()) {
    ResultPtr result(new Result(getPointer(), member, object));
    return result;
  }

  ResultPtr base = BaseContext::resolve(name);
  if (!base->isUndefined()) {
    return base;
  }

  return getMember(name);
}

// ----------------------------------------------------------------------------
TypePtr StaticMemberContext::getParentFunction() {
  return TypePtr();
}

// ----------------------------------------------------------------------------
TypePtr StaticMemberContext::self() {
  return object;
}

// ----------------------------------------------------------------------------
std::string StaticMemberContext::asString() const {
  std::ostringstream os;
  os << "<StaticMemberContext " << object->asString() << ">";
  return os.str();
}

// ----------------------------------------------------------------------------
