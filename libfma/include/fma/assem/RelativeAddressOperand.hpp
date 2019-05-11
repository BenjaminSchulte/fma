#ifndef __FMA_ASM_RELATIVEADDRESSOPERAND_H__
#define __FMA_ASM_RELATIVEADDRESSOPERAND_H__

#include "Operand.hpp"

namespace FMA {
namespace assem {

class RelativeAddressOperand : public Operand {
public:
  RelativeAddressOperand(Operand *left, Operand *right) : left(left), right(right) {};
  virtual ~RelativeAddressOperand() { delete left; delete right; };

  virtual std::string asString() const;
  virtual std::string getIdentifier() { return "(" + left->getIdentifier() + "+" + right->getIdentifier() + ")"; }

  virtual std::string getTypeName() const { return "RelativeAddress"; }

  virtual bool isResolvedAddress() const;
  virtual uint64_t asResolvedAddress() const;

  bool isSymbolReference() const;
  symbol::ReferencePtr asSymbolReference() const;

  virtual bool isWriteable() { return left->isWriteable(); }
  virtual bool isReadable() { return left->isReadable(); }

protected:
  Operand *left;
  Operand *right;
};

}
}

#endif