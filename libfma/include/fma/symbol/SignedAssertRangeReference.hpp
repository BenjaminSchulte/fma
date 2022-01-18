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

  static std::string ReferenceTypeId() { return "FMA::symbol::SignedAssertRangeReference"; }
  std::string referenceTypeId() const override { return ReferenceTypeId(); }

  virtual std::string asString() const override;
  virtual bool isConstant() const override { return other->isConstant(); }
  virtual uint64_t asConstant() const override { return other->asConstant(); }

  virtual uint64_t resolve(const plugin::MemorySymbolMap *map, bool &valid) const override;

  bool serialize(FMA::output::DynamicBuffer &) const override;
  
protected:
  ReferencePtr other;
  int64_t min;
  int64_t max;
};

}
}

#endif
