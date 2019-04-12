#ifndef __FMA_TYPES_DECORATOR_H__
#define __FMA_TYPES_DECORATOR_H__

#include "Decoratable.hpp"
#include "../interpret/ParameterList.hpp"
#include <vector>

namespace FMA {
namespace ast {
  typedef std::shared_ptr<class MacroParameter> MacroParameterPtr;
  typedef std::shared_ptr<class Statement> StatementPtr;
  typedef std::vector<MacroParameterPtr> MacroParameterList;
}
namespace interpret {
  typedef std::shared_ptr<class BaseContext> ContextPtr;
}
namespace types {

class Decorator : public Decoratable {
public:
  Decorator(const TypePtr &callback, const interpret::GroupedParameterList &parameters);

  virtual interpret::ResultPtr call(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameters);
  virtual interpret::ResultPtr callInDecorator(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameters, DecoratorCallType type);
  // virtual interpret::ResultPtr invokeDecoratorIfPresent(const interpret::ContextPtr &callContext, const interpret::GroupedParameterList &params, DecoratorCallType type);
  virtual std::string asString() const;
  virtual void dump(const std::string &prefix);

  virtual bool hasMember(const std::string &name) const;
  virtual TypePtr getMember(const std::string &name) const;
  virtual DecoratorCallType getDecoratorCallType() const { return callback->getDecoratorCallType(); }

  DecoratorPtr clone() const;

protected:
  interpret::GroupedParameterList parameters;
  TypePtr callback;
};

}
}

#endif
