#include <fma/types/Pointer.hpp>
#include <fma/types/Undefined.hpp>
#include <fma/core/Boolean.hpp>
#include <sstream>
#include <iostream>

using namespace FMA;
using namespace FMA::types;
using namespace FMA::core;

// ----------------------------------------------------------------------------
Pointer::Pointer(const TypePtr &child)
  : child(child)
{
}

// ----------------------------------------------------------------------------
std::string Pointer::asString() const {
  if (child->isPointer()) {
    return child->asString();
  }

  std::ostringstream os;
  os << "<Pointer " << child->asString() << ">";
  return os.str();
}

// ----------------------------------------------------------------------------
bool Pointer::hasMember(const std::string &name) const {
  if (child->isPointer()) {
    return child->hasMember(name);
  }

  if (name == "call") {
    return true;
  } else if (name == "nil?") {
    return true;
  }

  return false;
}

// ----------------------------------------------------------------------------
TypePtr Pointer::getMember(const std::string &name) const {
  if (child->isPointer()) {
    return child->getMember(name);
  }

  if (name == "call") {
    return child;
  } else if (name == "nil?") {
    return BooleanClass::macroFor(false);
  }

  return TypePtr(new Undefined());
}

// ----------------------------------------------------------------------------

