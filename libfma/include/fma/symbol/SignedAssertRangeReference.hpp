#ifndef __FMA_SYMBOL_SIGNEDASSERTRANGEREFERENCE_H__
#define __FMA_SYMBOL_SIGNEDASSERTRANGEREFERENCE_H__

#include "Reference.hpp"

namespace FMA {
namespace symbol {

class SignedAssertRangeReference : public Reference {
public:
  SignedAssertRangeReference(const ReferencePtr &other, int64_t min, int64_t max)
    : other(other)
    , min(min)
    , max(max) {}
  virtual ~SignedAssertRangeReference() {}

  virtual std::string asString() const;
  virtual bool isConstant() const { return other->isConstant(); }
  virtual uint64_t asConstant() const { return other->asConstant(); }

  virtual uint64_t resolve(const plugin::MemorySymbolMap *map, bool &valid) const;
  
protected:
  ReferencePtr other;
  int64_t min;
  int64_t max;
};

}
}

#endif
