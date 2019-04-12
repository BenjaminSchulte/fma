#include <memory/MemoryDeclarationClass.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/InstanceContext.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/types/InternalValue.hpp>

#include <iostream>

using namespace FMA;
using namespace FMA::memory;
using namespace FMA::types;
using namespace FMA::symbol;
using namespace FMA::interpret;

// ----------------------------------------------------------------------------
ClassPtr MemoryDeclarationClass::create(const RootModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("MemoryDeclaration", "MemoryDeclaration"));
  klass->extends(ClassObject);

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", MemoryDeclarationClass::initialize)));

  proto->setMember("+", TypePtr(new InternalFunctionValue("+", MemoryDeclarationClass::op_add)));
  proto->setMember("-", TypePtr(new InternalFunctionValue("-", MemoryDeclarationClass::op_sub)));
  proto->setMember("/", TypePtr(new InternalFunctionValue("/", MemoryDeclarationClass::op_div)));
  proto->setMember("*", TypePtr(new InternalFunctionValue("*", MemoryDeclarationClass::op_mul)));
  proto->setMember("%", TypePtr(new InternalFunctionValue("%", MemoryDeclarationClass::op_rem)));
  proto->setMember("&", TypePtr(new InternalFunctionValue("&", MemoryDeclarationClass::op_and)));
  proto->setMember("|", TypePtr(new InternalFunctionValue("|", MemoryDeclarationClass::op_or)));
  proto->setMember("^", TypePtr(new InternalFunctionValue("^", MemoryDeclarationClass::op_xor)));
  proto->setMember("<<", TypePtr(new InternalFunctionValue("<<", MemoryDeclarationClass::op_lshift)));
  proto->setMember(">>", TypePtr(new InternalFunctionValue(">>", MemoryDeclarationClass::op_rshift)));

  root->setMember("MemoryDeclaration", klass);
  return klass;
}

// ----------------------------------------------------------------------------
ResultPtr MemoryDeclarationClass::initialize(const ContextPtr &context, const GroupedParameterList &) {
  std::cout << "CREATE DECLARATION" << std::endl;

  return Result::executed(context, context->self());
}

// ---------------------------------------------------------------------------
ResultPtr MemoryDeclarationClass::calculation(const std::string &op, const ContextPtr &context, const GroupedParameterList &parameter) {
  TypePtr left = context->self()->getMember("to_sym");
  if (left->isUndefined()) {
    return ResultPtr(new Result());
  }

  GroupedParameterList empty;
  ContextPtr leftContext(new InstanceContext(context->getInterpreter(), context->self()->asObject(), "to_sym"));
  TypePtr leftResult = left->call(leftContext, empty)->get();
  if (leftResult->isUndefined()) {
    return ResultPtr(new Result());
  }

  TypePtr callback = leftResult->getMember(op);
  if (callback->isUndefined()) {
    return ResultPtr(new Result());
  }

  ContextPtr callContext(new InstanceContext(context->getInterpreter(), leftResult->asObject(), op));
  return callback->call(callContext, parameter);
}

// ---------------------------------------------------------------------------
ResultPtr MemoryDeclarationClass::op_add(const ContextPtr &context, const GroupedParameterList& params) {
  return calculation("+", context, params);
}

// ---------------------------------------------------------------------------
ResultPtr MemoryDeclarationClass::op_sub(const ContextPtr &context, const GroupedParameterList& params) {
  return calculation("-", context, params);
}

// ---------------------------------------------------------------------------
ResultPtr MemoryDeclarationClass::op_div(const ContextPtr &context, const GroupedParameterList& params) {
  return calculation("/", context, params);
}

// ---------------------------------------------------------------------------
ResultPtr MemoryDeclarationClass::op_mul(const ContextPtr &context, const GroupedParameterList& params) {
  return calculation("*", context, params);
}

// ---------------------------------------------------------------------------
ResultPtr MemoryDeclarationClass::op_rem(const ContextPtr &context, const GroupedParameterList& params) {
  return calculation("%", context, params);
}

// ---------------------------------------------------------------------------
ResultPtr MemoryDeclarationClass::op_and(const ContextPtr &context, const GroupedParameterList& params) {
  return calculation("&", context, params);
}

// ---------------------------------------------------------------------------
ResultPtr MemoryDeclarationClass::op_or(const ContextPtr &context, const GroupedParameterList& params) {
  return calculation("|", context, params);
}

// ---------------------------------------------------------------------------
ResultPtr MemoryDeclarationClass::op_xor(const ContextPtr &context, const GroupedParameterList& params) {
  return calculation("^", context, params);
}

// ---------------------------------------------------------------------------
ResultPtr MemoryDeclarationClass::op_lshift(const ContextPtr &context, const GroupedParameterList& params) {
  return calculation("<<", context, params);
}

// ---------------------------------------------------------------------------
ResultPtr MemoryDeclarationClass::op_rshift(const ContextPtr &context, const GroupedParameterList& params) {
  return calculation(">>", context, params);
}

