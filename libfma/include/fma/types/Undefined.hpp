#ifndef __FMA_TYPES_UNDEFINED_H__
#define __FMA_TYPES_UNDEFINED_H__

#include "Base.hpp"

namespace FMA {
namespace types {

class Undefined : public Base {
public:
  virtual bool isUndefined() const;
  virtual std::string asString() const;
  
  virtual interpret::ResultPtr callWithoutDecoratorTest(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter); 
};

extern TypePtr undefinedValue;

}
}

#endif
