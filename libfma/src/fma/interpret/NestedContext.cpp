#include <fma/Project.hpp>
#include <fma/interpret/Interpreter.hpp>
#include <fma/interpret/NestedContext.hpp>
#include <fma/interpret/Result.hpp>

using namespace FMA;
using namespace FMA::types;
using namespace FMA::interpret;

#include <iostream>
#include <sstream>

// ----------------------------------------------------------------------------
NestedContext::NestedContext(Interpreter *interpreter, const ContextPtr &current, const ContextPtr &parent)
  : BaseContext(interpreter)
  , current(current)
  , parent(parent)
{
}

// ----------------------------------------------------------------------------
ResultPtr NestedContext::getMember(const std::string &name) {
  return current->getMember(name);
}

// ----------------------------------------------------------------------------
ResultPtr NestedContext::setMember(const std::string &name, const TypePtr &value) {
  return current->setMember(name, value);
}

// ----------------------------------------------------------------------------
ResultPtr NestedContext::resolve(const std::string &name) {
  // std::cout << "\x1b[33mSEARCH FOR\x1b[m " << name << " IN " << asString() << std::endl;
  ResultPtr self = current->resolve(name);
  if (!self->isUndefined()) {
    // std::cout << "\x1b[32mFOUND\x1b[m " << name << " IN " << current->asString() << std::endl;
    return self;
  }

  ResultPtr other = parent->resolve(name);
  if (!other->isUndefined()) {
    // std::cout << "\x1b[32mFOUND\x1b[m " << name << " IN " << parent->asString() << std::endl;
    return other;
  }

  ResultPtr base = BaseContext::resolve(name);
  if (!base->isUndefined()) {
    // std::cout << "\x1b[32mFOUND\x1b[m " << name << " IN BASE" << std::endl;
    return base;
  }

  // std::cout << "\x1b[31mNOT FOUND\x1b[m " << name << " IN " << asString() << " RETURNING " << current->asString() << std::endl;

  return self;
}

// ----------------------------------------------------------------------------
TypePtr NestedContext::getParentFunction() {
  TypePtr left = current->getParentFunction();
  if (left) {
    return left;
  }

  return parent->getParentFunction();
}

// ----------------------------------------------------------------------------
TypePtr NestedContext::getDeclareLevelObject() {
  return current->getDeclareLevelObject();
}

// ----------------------------------------------------------------------------
ContextPtr NestedContext::getDeclareLevelContext() {
  return ContextPtr(new NestedContext(interpreter, current->getDeclareLevelContext(), parent));
}

// ----------------------------------------------------------------------------
ContextPtr NestedContext::getMacroDeclareLevelContext() {
  return ContextPtr(new NestedContext(interpreter, current->getMacroDeclareLevelContext(), parent));
}

// ----------------------------------------------------------------------------
ContextPtr NestedContext::withoutInstances() {
  ContextPtr main(current->withoutInstances());
  ContextPtr sub(parent->withoutInstances());

  if (main->isValidContext() && sub->isValidContext()) {
    return ContextPtr(new NestedContext(interpreter, main, sub));
  } else if (main->isValidContext()) {
    return main;
  } else {
    return sub;
  }
}

// ----------------------------------------------------------------------------
std::string NestedContext::asString() const {
  std::ostringstream os;
  os << "<NestedContext " << current->asString() << " PARENT " << parent->asString();
  os << ">";
  return os.str();
}

// ----------------------------------------------------------------------------
std::string NestedContext::getNameHint() const {
  std::string parentName = parent->getNameHint();

  std::ostringstream os;
  if (parentName.size()) {
    os << parentName << ".";
  }
  os << current->getNameHint();
  return os.str();
}

// ----------------------------------------------------------------------------
