#ifndef __FMA_ASM_STACKADDRESSOPERAND_H__
#define __FMA_ASM_STACKADDRESSOPERAND_H__

#include "Operand.hpp"

namespace FMA {
namespace assem {

class StackAddressOperand : public Operand {
public:
  StackAddressOperand(Operand *address) : address(address) {};
  virtual ~StackAddressOperand() { delete address; };

  virtual std::string asString() const;
  virtual std::string getIdentifier() { return "sp(" + address->getIdentifier() + ")"; }

  virtual std::string getTypeName() const { return "StackAddress"; }

  virtual bool isResolvedAddress() const { return address->isResolvedAddress(); }
  virtual uint64_t asResolvedAddress() const { return address->asResolvedAddress(); }

  virtual bool isSymbolReference() const { return address->isSymbolReference(); }
  symbol::ReferencePtr asSymbolReference() const { return address->asSymbolReference(); }

  virtual bool isWriteable() { return true; }
  virtual bool isReadable() { return true; }

protected:
  Operand *address;
};

}
}

#endif