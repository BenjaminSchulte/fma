#ifndef __FMA_SYMBOL_CALCULATEDNUMBER_H__
#define __FMA_SYMBOL_CALCULATEDNUMBER_H__

#include "Reference.hpp"

namespace FMA {
namespace symbol {

class CalculatedNumber : public Reference {
public:
  enum Operand {
    ADD,
    SUB,
    DIV,
    MUL,
    REM,
    AND,
    OR,
    XOR,
    LSHIFT,
    RSHIFT
  };

  CalculatedNumber(const ReferencePtr &left, Operand op, const ReferencePtr &right)
    : left(left), op(op), right(right) {}
  virtual ~CalculatedNumber() {}

  virtual std::string asString() const override;
  virtual bool isConstant() const override;
  virtual uint64_t asConstant() const override;
  uint64_t asConstant(uint64_t leftValue, uint64_t rightValue) const;

  virtual uint64_t resolve(const plugin::MemorySymbolMap *map, bool &valid) const override;

  bool serialize(FMA::output::DynamicBuffer &) const override;
  static ReferencePtr deserialize(Log *log, FMA::output::DynamicBuffer &);

protected:
  ReferencePtr left;
  Operand op;
  ReferencePtr right;
};

}
}

#endif
