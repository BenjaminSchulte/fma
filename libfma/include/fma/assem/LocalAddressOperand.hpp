#ifndef __FMA_ASM_LOCALADDRESSOPERAND_H__
#define __FMA_ASM_LOCALADDRESSOPERAND_H__

#include "Operand.hpp"

namespace FMA {
namespace assem {

class LocalAddressOperand : public Operand {
public:
  LocalAddressOperand(Operand *address) : address(address) {};
  virtual ~LocalAddressOperand() { delete address; };

  virtual std::string asString() const;
  virtual std::string getIdentifier() { return "la(" + address->getIdentifier() + ")"; }

  virtual std::string getTypeName() const { return "LocalAddress"; }

  virtual bool isResolvedAddress() const { return address->isResolvedAddress(); }
  virtual uint64_t asResolvedAddress() const { return address->asResolvedAddress(); }

  bool isSymbolReference() const { return address->isSymbolReference(); }
  symbol::ReferencePtr asSymbolReference() const { return address->asSymbolReference(); }

  virtual bool isWriteable() { return true; }
  virtual bool isReadable() { return true; }

protected:
  Operand *address;
};

}
}

#endif