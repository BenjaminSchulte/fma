#include <fma/ast/Statement.hpp>
#include <fma/types/Decorator.hpp>
#include <fma/types/Block.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/ObjectContext.hpp>
#include <fma/interpret/NestedContext.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <sstream>
#include <iostream>

using namespace FMA;
using namespace FMA::interpret;
using namespace FMA::ast;
using namespace FMA::types;


// ----------------------------------------------------------------------------
DecoratorCall::DecoratorCall(const DecoratablePtr &macro, const ContextPtr &callContext, const interpret::GroupedParameterList &parameters, DecoratorCallType type)
  : macro(macro)
  , callContext(callContext)
  , parameters(parameters)
  , type(type)
{
}

// ----------------------------------------------------------------------------
ResultPtr DecoratorCall::call(const ContextPtr&, const interpret::GroupedParameterList&) {
  if (Decoratable::isCorrectDecoratorMode(macro->getDecoratorCallType(), type)) {
    return macro->callInDecoratorIfCorrectMode(callContext, parameters, type);
  }

  return Decoratable::skipDecorator(callContext, parameters);
}

// ----------------------------------------------------------------------------
std::string DecoratorCall::asString() const {
  std::ostringstream os;
  os << "<DecoratorCall " << macro->asString();
  os << " " << parameters.asString(callContext);
  os << ">";
  return os.str();
}

// ----------------------------------------------------------------------------
ResultPtr Decoratable::skipDecorator(const ContextPtr &context, const interpret::GroupedParameterList &params) {
  const interpret::TypeList &blocks = params.only_blocks();
  if (!blocks.size()) {
    return ResultPtr(new Result());
  }

  return blocks.front()->call(context, params);
}

// ----------------------------------------------------------------------------
ResultPtr Decoratable::callWithoutDecoratorTest(const ContextPtr &callContext, const interpret::GroupedParameterList &params) {
  return invokeDecoratorIfPresent(callContext, params, DECORATORCALL_INNER);
}

// ----------------------------------------------------------------------------
ResultPtr Decoratable::invokeDecoratorIfPresent(const ContextPtr &callContext, const interpret::GroupedParameterList &params, DecoratorCallType type) {
  if (!decorator) {
    return callInDecoratorIfCorrectMode(callContext, params, type);
  }

  return invokeDecorator(callContext, params, type);
}

// ----------------------------------------------------------------------------
ResultPtr Decoratable::callInDecoratorIfCorrectMode(const ContextPtr &context, const interpret::GroupedParameterList &params, DecoratorCallType type) {
  if (isCorrectDecoratorMode(getDecoratorCallType(), type)) {
    return callInDecorator(context, params, type);
  }

  const interpret::TypeList &blocks = params.only_blocks();
  if (!blocks.size()) {
    return ResultPtr(new Result());
  }

  interpret::GroupedParameterList emptyParams;
  TypePtr block = blocks.front();
  if (block->isDecoratable()) {
    return std::dynamic_pointer_cast<Decoratable>(block)->callInDecoratorIfCorrectMode(context, emptyParams, type);
  } else {
    return block->call(context, emptyParams);
  }
}

// ----------------------------------------------------------------------------
ResultPtr Decoratable::invokeDecorator(const ContextPtr &callContext, const interpret::GroupedParameterList &params, DecoratorCallType type) {
  TypePtr selfCall(new DecoratorCall(std::dynamic_pointer_cast<Decoratable>(getPointer()), callContext, params, type));

  interpret::GroupedParameterList localParams;
  localParams.push_back(interpret::Parameter(interpret::Parameter::BLOCK, selfCall));
  
  return invokeDecoratorWithParams(callContext, localParams, type);
}

// ----------------------------------------------------------------------------
ResultPtr Decoratable::invokeDecoratorWithParams(const ContextPtr &callContext, const interpret::GroupedParameterList &params, DecoratorCallType type) {
  if (decorator->isDecoratable()) {
    return std::dynamic_pointer_cast<Decoratable>(decorator)->invokeDecoratorIfPresent(callContext, params, type);
  } else {
    return decorator->callWithoutDecoratorTest(callContext, params);
  }
}

// ----------------------------------------------------------------------------

