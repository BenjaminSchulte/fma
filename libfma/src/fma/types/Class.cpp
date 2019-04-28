#include <fma/types/Class.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/types/Object.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/ObjectContext.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/Parameter.hpp>
#include <sstream>
#include <iostream>

using namespace FMA;
using namespace FMA::types;
using namespace FMA::interpret;

// ----------------------------------------------------------------------------
Class::Class(const std::string &name, const std::string &fullName)
  : name(name)
  , fullName(fullName)
  , prototype(NULL)
{
}

// ----------------------------------------------------------------------------
const ClassPrototypePtr &Class::getPrototype() {
  if (!prototype) {
    prototype = ClassPrototypePtr(new ClassPrototype(std::dynamic_pointer_cast<Class>(getPointer())));
  }

  return prototype;
}

// ----------------------------------------------------------------------------
void Class::extends(const ClassPtr &other) {
  parent = other;
}

// ----------------------------------------------------------------------------
bool Class::isInstanceOf(const ClassPtr &other) const {
  if (this == other.get()) {
    return true;
  }

  if (parent) {
    return parent->isInstanceOf(other);
  }

  return false;
}

// ----------------------------------------------------------------------------
bool Class::isInstanceOf(const std::string &other) const {
  if (fullName == other) {
    return true;
  }

  if (parent) {
    return parent->isInstanceOf(other);
  }

  return false;
}

// ----------------------------------------------------------------------------
bool Class::hasPrototypeMember(const std::string &name) {
  if (getPrototype()->hasMember(name)) {
    return true;
  }

  if (parent) {
    return parent->hasPrototypeMember(name);
  }

  return false;
}

// ----------------------------------------------------------------------------
bool Class::hasMember(const std::string &name) const {
  if (Base::hasMember(name)) {
    return true;
  }

  if (parent) {
    return parent->hasMember(name);
  }

  return false;
}

// ----------------------------------------------------------------------------
ResultPtr Class::callWithoutDecoratorTest(const ContextPtr &context, const GroupedParameterList&) {
  return Result::executed(context, getPointer());
}

// ----------------------------------------------------------------------------
TypePtr Class::getMember(const std::string &name) const {
  TypePtr result = Base::getMember(name);

  if (result->isUndefined() && parent) {
    result = parent->getMember(name);
  }

  return result;
}

// ----------------------------------------------------------------------------
TypePtr Class::getPrototypeMember(const std::string &name) {
  TypePtr result = getPrototype()->getMember(name);

  if (result->isUndefined() && parent) {
    result = parent->getPrototypeMember(name);
  }

  return result;
}

// ----------------------------------------------------------------------------
ObjectPtr Class::createInstance(const ContextPtr &context, const GroupedParameterList &parameters) {
  ClassPtr klass = std::dynamic_pointer_cast<Class>(getPointer());
  ObjectPtr object(new Object(klass));

  TypePtr constructor = object->getMember("initialize");
  if (!constructor->isUndefined()) {
    ContextPtr objectContext(new ObjectContext(context->getInterpreter(), object));
    constructor->call(objectContext, parameters);
  }

  return object;
}

// ----------------------------------------------------------------------------
std::string Class::asString() const {
  std::ostringstream os;
  os << "<Class " << name << ">";
  return os.str();
}

// ----------------------------------------------------------------------------
void Class::dump(const std::string &prefix) {
  std::cout << prefix << "class " << name << std::endl;
  for (auto &member : members) {
    std::cout << prefix << "  " << member.first << " = " << member.second->asString() << std::endl;
  }
  std::cout << prefix << "end" << std::endl;
}

// ----------------------------------------------------------------------------

