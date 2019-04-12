#include <fma/Project.hpp>
#include <fma/interpret/Interpreter.hpp>
#include <fma/interpret/NameHintContext.hpp>
#include <fma/interpret/Result.hpp>

using namespace FMA;
using namespace FMA::types;
using namespace FMA::interpret;

#include <iostream>
#include <sstream>

// ----------------------------------------------------------------------------
NameHintContext::NameHintContext(Interpreter *interpreter, const std::string &name, const ContextPtr &parent)
  : BaseContext(interpreter)
  , name(name)
  , parent(parent)
{
}

// ----------------------------------------------------------------------------
ResultPtr NameHintContext::getMember(const std::string &name) {
  return parent->getMember(name);
}

// ----------------------------------------------------------------------------
ResultPtr NameHintContext::setMember(const std::string &name, const TypePtr &value) {
  return parent->setMember(name, value);
}

// ----------------------------------------------------------------------------
ResultPtr NameHintContext::resolve(const std::string &name) {
  return parent->resolve(name);
}

// ----------------------------------------------------------------------------
TypePtr NameHintContext::getDeclareLevelObject() {
  return parent->getDeclareLevelObject();
}


// ----------------------------------------------------------------------------
std::string NameHintContext::asString() const {
  std::ostringstream os;
  os << "<NameHintContext " << name << ">";
  return os.str();
}

// ----------------------------------------------------------------------------
std::string NameHintContext::getNameHint() const {
  std::string parentName = parent->getNameHint();

  std::ostringstream os;
  if (parentName.size()) {
    os << parentName << ".";
  }
  os << name;
  return os.str();
}

// ----------------------------------------------------------------------------
