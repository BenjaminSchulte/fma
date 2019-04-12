#ifndef __FMA_TYPES_DECORATABLE_H__
#define __FMA_TYPES_DECORATABLE_H__

#include "Base.hpp"
#include "../interpret/ParameterList.hpp"

namespace FMA {
namespace types {

class DecoratorCall : public Base {
public:
  DecoratorCall(const DecoratablePtr &macro, const interpret::ContextPtr &callContext, const interpret::GroupedParameterList &parameters, DecoratorCallType type);
  virtual interpret::ResultPtr call(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameters);
  virtual std::string asString() const;

  virtual const DecoratablePtr &getDecoratable() const { return macro; }
  virtual DecoratorCallType getDecoratorCallType() const { return type; }
  
protected:
  DecoratablePtr macro;
  interpret::ContextPtr callContext;
  interpret::GroupedParameterList parameters;
  DecoratorCallType type;
};

class Decoratable : public Base {
public:
  inline void setDecorator(const TypePtr &object) {
    this->decorator = object;
  }

  inline bool hasDecorator() const {
    return !!decorator;
  }

  inline const TypePtr &getDecorator() const {
    return decorator;
  }

  virtual bool isDecoratable() const { return true; }

  virtual interpret::ResultPtr callWithoutDecoratorTest(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameters);
  virtual interpret::ResultPtr invokeDecoratorIfPresent(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameters, DecoratorCallType type);
  virtual interpret::ResultPtr invokeDecorator(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameters, DecoratorCallType type);
  virtual interpret::ResultPtr invokeDecoratorWithParams(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameters, DecoratorCallType type);
  virtual interpret::ResultPtr callInDecoratorIfCorrectMode(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameters, DecoratorCallType type);
  virtual interpret::ResultPtr callInDecorator(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameters, DecoratorCallType type) = 0;

  static inline bool isCorrectDecoratorMode(DecoratorCallType macro, DecoratorCallType expect) {
    uint8_t macroType = (uint8_t)macro;
    if (macroType == 0) {
      macroType = (uint8_t)DECORATORCALL_INNER_AND_OUTER;
    }

    return !!(macroType & expect);
  }

  static interpret::ResultPtr skipDecorator(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameters);

protected:
  TypePtr decorator;
};

}
}

#endif
