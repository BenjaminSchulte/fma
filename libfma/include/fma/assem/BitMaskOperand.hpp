#ifndef __FMA_ASM_BITMASKOPERAND_H__
#define __FMA_ASM_BITMASKOPERAND_H__

#include "Operand.hpp"

namespace FMA {
namespace assem {

class BitMaskOperand : public Operand {
public:
  BitMaskOperand(Operand *address, uint8_t lsh, uint8_t rsh, uint64_t mask);
  BitMaskOperand(Operand *address, Operand *lsh, Operand *rsh, uint64_t mask);
  virtual ~BitMaskOperand() { delete address; delete lsh; delete rsh; };

  virtual std::string asString() const;
  virtual std::string getIdentifier() { return "(" + address->getIdentifier() + "&#)"; }

  virtual std::string getTypeName() const { return "BitMask"; }

  virtual bool isResolvedAddress() const { return address->isResolvedAddress(); }
  virtual uint64_t asResolvedAddress() const { return address->asResolvedAddress(); }

  bool isSymbolReference() const { return address->isSymbolReference(); }
  symbol::ReferencePtr asSymbolReference() const { return address->asSymbolReference(); }

  inline Operand *getAddress() const { return address; }
  inline Operand *getLsh() const { return lsh; }
  inline Operand *getRsh() const { return rsh; }

  virtual bool isWriteable() { return address->isWriteable(); }
  virtual bool isReadable() { return address->isReadable(); }

protected:
  Operand *address;
  Operand *lsh;
  Operand *rsh;
  uint64_t mask;
};

}
}

#endif