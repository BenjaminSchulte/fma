#include <fma/Project.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/interpret/Interpreter.hpp>
#include <fma/interpret/ProjectContext.hpp>
#include <fma/interpret/Result.hpp>

using namespace FMA;
using namespace FMA::types;
using namespace FMA::interpret;

#include <iostream>

// ----------------------------------------------------------------------------
ProjectContext::ProjectContext(Interpreter *interpreter)
  : BaseContext(interpreter)
{
}

// ----------------------------------------------------------------------------
TypePtr ProjectContext::getDeclareLevelObject() {
  return interpreter->getProject()->root();
}

// ----------------------------------------------------------------------------
ResultPtr ProjectContext::getMember(const std::string &name) {
  RootModulePtr root = interpreter->getProject()->root();

  ResultPtr result(new Result(getPointer(), root->getMember(name)));
  result->makeAssignable(getPointer(), name);
  return result;
}

// ----------------------------------------------------------------------------
ResultPtr ProjectContext::setMember(const std::string &name, const TypePtr &value) {
  RootModulePtr root = interpreter->getProject()->root();

  root->setMember(name, value);

  ResultPtr result(new Result(getPointer(), value));
  result->makeAssignable(getPointer(), name);
  return result;
}

// ----------------------------------------------------------------------------
ResultPtr ProjectContext::resolve(const std::string &name) {
  RootModulePtr root = interpreter->getProject()->root();

  ResultPtr result(new Result(getPointer(), root->getMember(name)));
  result->makeAssignable(getPointer(), name);

  return result;
}

// ----------------------------------------------------------------------------
std::string ProjectContext::asString() const {
  return "<ProjectContext>";
}

// ----------------------------------------------------------------------------
