#include <fma/core/Class.hpp>
#include <fma/core/String.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/types/Object.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/types/Pointer.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/PrototypeContext.hpp>
#include <fma/interpret/ObjectContext.hpp>
#include <fma/interpret/NestedContext.hpp>
#include <fma/interpret/ClassContext.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/Project.hpp>

#include <iostream>

using namespace FMA;
using namespace FMA::core;
using namespace FMA::types;
using namespace FMA::interpret;

// ----------------------------------------------------------------------------
ClassPtr ClassClass::create(const RootModulePtr &root, const ClassPtr &Object) {
  ClassPtr klass = ClassPtr(new Class("Class", "Class"));
  klass->extends(Object);

  klass->setMember("new", TypePtr(new InternalFunctionValue("new", ClassClass::newInstance)));
  klass->setMember("name", TypePtr(new InternalFunctionValue("name", ClassClass::name)));
  klass->setMember("class_eval", TypePtr(new InternalFunctionValue("class_eval", ClassClass::class_eval)));
  klass->setMember("instance_eval", TypePtr(new InternalFunctionValue("instance_eval", ClassClass::instance_eval)));
  klass->setMember("include_class", TypePtr(new InternalFunctionValue("include", ClassClass::include)));
  klass->setMember("define_method", TypePtr(new InternalFunctionValue("define_method", ClassClass::define_method)));
  klass->setMember("define_prototype_method", TypePtr(new InternalFunctionValue("define_prototype_method", ClassClass::define_prototype_method)));

  root->setMember("Class", klass);
  return klass;
}

// ----------------------------------------------------------------------------
ResultPtr ClassClass::newInstance(const ContextPtr &context, const GroupedParameterList &parameter) {
  ClassPtr self = context->self()->asClass();
  if (!self) {
    return ResultPtr(new Result());
  }

  return Result::executed(context, self->createInstance(context, parameter));
}

// ----------------------------------------------------------------------------
ResultPtr ClassClass::name(const ContextPtr &context, const GroupedParameterList &) {
  ClassPtr self = context->self()->asClass();
  if (!self) {
    return ResultPtr(new Result());
  }

  return StringClass::createInstance(context, self->getName());
}

// ----------------------------------------------------------------------------
ResultPtr ClassClass::class_eval(const ContextPtr &context, const GroupedParameterList &parameter) {
  ClassPtr self = context->self()->asClass();
  if (!self) {
    return ResultPtr(new Result());
  }

  const TypeList &blocks = parameter.only_blocks();
  if (!blocks.size()) {
    return ResultPtr(new Result());
  }

  ContextPtr ctx(new ObjectContext(context->getInterpreter(), self));
  GroupedParameterList emptyParams;
  return blocks.front()->call(ctx, emptyParams);
}

// ----------------------------------------------------------------------------
ResultPtr ClassClass::include(const ContextPtr &context, const GroupedParameterList &parameter) {
  ClassPtr self = context->self()->asClass();
  if (!self) {
    return ResultPtr(new Result());
  }

  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    return ResultPtr(new Result());
  }

  ClassPtr other = args.front()->asClass();
  if (!other) {
    context->log().error() << "include_class parameter must be a class";
    return ResultPtr(new Result());
  }

  self->extends(other);
  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
ResultPtr ClassClass::instance_eval(const ContextPtr &context, const GroupedParameterList &parameter) {
  ClassPtr self = context->self()->asClass();
  if (!self) {
    return ResultPtr(new Result());
  }

  const TypeList &blocks = parameter.only_blocks();
  if (!blocks.size()) {
    return ResultPtr(new Result());
  }

  ContextPtr ctx(new PrototypeContext(context->getInterpreter(), self));
  GroupedParameterList emptyParams;
  return blocks.front()->call(ctx, emptyParams);
}

// ----------------------------------------------------------------------------
ResultPtr ClassClass::define_method(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  const TypeList &blocks = parameter.only_blocks();
  if (!args.size() || !blocks.size()) {
    return ResultPtr(new Result());
  }

  TypePtr macro = blocks.front();
  while (macro->isPointer()) {
    macro =macro->asPointer()->getChild();
  }
  context->self()->setMember(args.front()->convertToString(context), macro);

  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
ResultPtr ClassClass::define_prototype_method(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  const TypeList &blocks = parameter.only_blocks();
  if (!args.size() || !blocks.size()) {
    return ResultPtr(new Result());
  }

  TypePtr macro = blocks.front();
  while (macro->isPointer()) {
    macro =macro->asPointer()->getChild();
  }
  
  ClassPtr self = context->self()->asClass();
  self->getPrototype()->setMember(args.front()->convertToString(context), macro);

  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
