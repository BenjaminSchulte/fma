#ifndef __FMA_ASM_MEMORYADDRESSOPERAND_H__
#define __FMA_ASM_MEMORYADDRESSOPERAND_H__

#include "Operand.hpp"

namespace FMA {
namespace assem {

class MemoryAddressOperand : public Operand {
public:
  MemoryAddressOperand(Operand *address) : address(address) {};
  virtual ~MemoryAddressOperand() { delete address; };

  virtual std::string asString() const;
  virtual std::string getIdentifier() { return "[" + address->getIdentifier() + "]"; }

  virtual std::string getTypeName() const { return "MemoryAddress"; }

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