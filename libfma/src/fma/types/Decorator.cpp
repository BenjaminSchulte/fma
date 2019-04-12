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
Decorator::Decorator(const TypePtr &callback, const interpret::GroupedParameterList &parameters)
  : parameters(parameters)
  , callback(callback)
{
}

// ----------------------------------------------------------------------------
DecoratorPtr Decorator::clone() const {
  DecoratorPtr decorator(new Decorator(callback, parameters));
  return decorator;
}

// ----------------------------------------------------------------------------
void Decorator::dump(const std::string &prefix) {
  std::cout << prefix << "@DECORATOR " << std::endl;
}

// ----------------------------------------------------------------------------
ResultPtr Decorator::call(const ContextPtr &context, const interpret::GroupedParameterList &parameters) {
  return callWithoutDecoratorTest(context, parameters);
}

// ----------------------------------------------------------------------------
ResultPtr Decorator::callInDecorator(const ContextPtr &callContext, const interpret::GroupedParameterList &params, DecoratorCallType type) {
  interpret::GroupedParameterList localParams(parameters);
  localParams.merge(params);

  if (callback->isDecoratable()) {
    return std::dynamic_pointer_cast<Decoratable>(callback)->invokeDecoratorIfPresent(callContext, localParams, type);
  } else {
    return callback->callWithoutDecoratorTest(callContext, localParams);
  }
}

// ----------------------------------------------------------------------------
/*
ResultPtr Decorator::invokeDecoratorIfPresent(const ContextPtr &callContext, const interpret::GroupedParameterList &params, DecoratorCallType type) {
  if (Decoratable::isCorrectDecoratorMode(callback->getDecoratorCallType(), type)) {
    return Decoratable::invokeDecoratorIfPresent(callContext, params, type);
  }


}
*/

// ----------------------------------------------------------------------------
std::string Decorator::asString() const {
  std::ostringstream os;
  os << "<Decorator " << callback->asString() << ">";
  return os.str();
}

// ----------------------------------------------------------------------------
bool Decorator::hasMember(const std::string &name) const {
  return Base::hasMember(name);
}

// ----------------------------------------------------------------------------
TypePtr Decorator::getMember(const std::string &name) const {
  return Base::getMember(name);
}


