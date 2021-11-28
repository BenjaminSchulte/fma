#ifndef __FMA_SYMBOL_CONSTANTNUMBER_H__
#define __FMA_SYMBOL_CONSTANTNUMBER_H__

#include "Reference.hpp"

namespace FMA {
namespace symbol {

class ConstantNumber : public Reference {
public:
  ConstantNumber(const uint64_t &number) : number(number) {}
  virtual ~ConstantNumber() {}

  virtual std::string asString() const override;
  virtual bool isConstant() const override { return true; }
  virtual uint64_t asConstant() const override { return number; }

  virtual uint64_t resolve(const plugin::MemorySymbolMap *map, bool &valid) const override;

  bool serialize(FMA::output::DynamicBuffer &) const override;
  static ReferencePtr deserialize(Log *log, FMA::output::DynamicBuffer &, uint8_t byteSize);
  
protected:
  uint64_t number;
};

}
}

#endif
