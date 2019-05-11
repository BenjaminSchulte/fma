#ifndef __FMA_ASM_PCRELATIVEADDRESSOPERAND_H__
#define __FMA_ASM_PCRELATIVEADDRESSOPERAND_H__

#include "Operand.hpp"

namespace FMA {
namespace assem {

class PcRelativeAddressOperand : public Operand {
public:
  PcRelativeAddressOperand(Operand *address) : address(address) {};
  virtual ~PcRelativeAddressOperand() { delete address; };

  virtual std::string asString() const;
  virtual std::string getIdentifier() { return "pcrel(" + address->getIdentifier() + ")"; }

  virtual std::string getTypeName() const { return "PcRelativeAddress"; }

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