#include <fma/Project.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/types/Object.hpp>
#include <fma/types/Class.hpp>
#include <fma/interpret/Interpreter.hpp>
#include <fma/interpret/InstanceContext.hpp>
#include <fma/interpret/Result.hpp>

using namespace FMA;
using namespace FMA::types;
using namespace FMA::interpret;

#include <iostream>
#include <sstream>

// ----------------------------------------------------------------------------
InstanceContext::InstanceContext(Interpreter *interpreter, const ObjectPtr &object, const std::string &name)
  : BaseContext(interpreter)
  , name(name)
  , object(object)
{
}

// ----------------------------------------------------------------------------
TypePtr InstanceContext::getDeclareLevelObject() {
  return object;
}

// ----------------------------------------------------------------------------
ResultPtr InstanceContext::getMember(const std::string &name) {
  if (name == "self") {
    return ResultPtr(new Result(getPointer(), object));
  }

  ResultPtr result(new Result(getPointer(), object->getMember(name)));
  result->makeAssignable(getPointer(), name);
  return result;
}

// ----------------------------------------------------------------------------
ResultPtr InstanceContext::setMember(const std::string &name, const TypePtr &value) {
  object->setMember(name, value);

  ResultPtr result(new Result(getPointer(), value));
  result->makeAssignable(getPointer(), name);
  return result;
}

// ----------------------------------------------------------------------------
ResultPtr InstanceContext::resolve(const std::string &name) {
  if (name == "super") {
    ClassPtr parent = object->getClass()->getParent();

    if (parent) {
      TypePtr item = parent->getPrototypeMember(this->name);
      if (!item->isUndefined()) {
        ResultPtr result(new Result(getPointer(), item, object));
        return result;
      }
    }
  }

  TypePtr member = object->getClass()->getPrototypeMember(name);
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
TypePtr InstanceContext::getParentFunction() {
  if (object->isObjectOfType("Function")) {
    return object;
  }

  return TypePtr();
}

// ----------------------------------------------------------------------------
TypePtr InstanceContext::self() {
  return object;
}

// ----------------------------------------------------------------------------
std::string InstanceContext::asString() const {
  std::ostringstream os;
  os << "<InstanceContext " << object->asString() << ">";
  return os.str();
}

// ----------------------------------------------------------------------------
