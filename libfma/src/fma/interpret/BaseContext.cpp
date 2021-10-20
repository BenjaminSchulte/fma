#include <fma/interpret/Interpreter.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/ProjectContext.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/types/Undefined.hpp>
#include <fma/ast/Node.hpp>
#include <fma/Project.hpp>

#include <iostream>

using namespace FMA;
using namespace FMA::interpret;
using namespace FMA::types;
using namespace FMA::ast;

// ----------------------------------------------------------------------------
BaseContext::BaseContext(Interpreter *interpreter)
  : interpreter(interpreter)
{
}

// ----------------------------------------------------------------------------
TypePtr BaseContext::getParentFunction() {
  return TypePtr();
}

// ----------------------------------------------------------------------------
TypePtr BaseContext::self() {
  return TypePtr(new Undefined());
}

// ----------------------------------------------------------------------------
TypePtr BaseContext::getDeclareLevelObject() {
  return TypePtr(new Undefined());
}

// ----------------------------------------------------------------------------
Project *BaseContext::getProject() const {
  return interpreter->getProject();
}

// ----------------------------------------------------------------------------
ContextPtr BaseContext::getStatementLevelContext() {
  return getPointer();
}

// ----------------------------------------------------------------------------
ContextPtr BaseContext::getDeclareLevelContext() {
  return getPointer();
}

// ----------------------------------------------------------------------------
ContextPtr BaseContext::getRootLevelContext() {
  return ContextPtr(new ProjectContext(interpreter));
}

// ----------------------------------------------------------------------------
ResultPtr BaseContext::getMember(const std::string&) {
  return ResultPtr(new Result(getPointer(), TypePtr(new Undefined())));
}

// ----------------------------------------------------------------------------
ResultPtr BaseContext::setMember(const std::string&, const TypePtr&) {
  interpreter->getProject()->log().error() << "Unable to set member in " << asString();
  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
ResultPtr BaseContext::resolve(const std::string &name) {
  return getMember(name);
}

// ----------------------------------------------------------------------------
std::string BaseContext::asString() const {
  return "<BaseContext>";
}

// ----------------------------------------------------------------------------
ResultPtr BaseContext::execute(const NodePtr &node) {
  ResultPtr result;
  NodePtr current = node;

  while (current) {
    result = current->execute(getPointer());
    if (result->isEarlyReturn()) {
      break;
    }

    current = current->getNextSibling();
  };

  return result;
}

// ----------------------------------------------------------------------------
std::string BaseContext::getNameHint() const {
  return "";
}

// ----------------------------------------------------------------------------
