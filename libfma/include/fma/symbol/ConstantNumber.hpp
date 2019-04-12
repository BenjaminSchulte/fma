#ifndef __FMA_SYMBOL_CONSTANTNUMBER_H__
#define __FMA_SYMBOL_CONSTANTNUMBER_H__

#include "Reference.hpp"

namespace FMA {
namespace symbol {

class ConstantNumber : public Reference {
public:
  ConstantNumber(const uint64_t &number) : number(number) {}
  virtual ~ConstantNumber() {}

  virtual std::string asString() const;
  virtual bool isConstant() const { return true; }
  virtual uint64_t asConstant() const { return number; }

  virtual uint64_t resolve(const plugin::MemorySymbolMap *map, bool &valid) const;
  
protected:
  uint64_t number;
};

}
}

#endif
