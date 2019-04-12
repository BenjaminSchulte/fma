#include <fma/types/Pointer.hpp>
#include <fma/types/Undefined.hpp>
#include <sstream>
#include <iostream>

using namespace FMA;
using namespace FMA::types;

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
  }

  return TypePtr(new Undefined());
}

// ----------------------------------------------------------------------------

