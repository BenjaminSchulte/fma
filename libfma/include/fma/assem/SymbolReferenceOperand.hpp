#ifndef __FMA_ASM_SYMBOLREFERENCEOPERAND_H__
#define __FMA_ASM_SYMBOLREFERENCEOPERAND_H__

#include "Operand.hpp"
#include "../symbol/Reference.hpp"

namespace FMA {
namespace assem {

class SymbolReferenceOperand : public Operand {
public:
  SymbolReferenceOperand(const symbol::ReferencePtr &reference) : reference(reference) {};
  virtual ~SymbolReferenceOperand() {};

  virtual std::string asString() const;
  virtual std::string getIdentifier() { return "#"; }

  virtual std::string getTypeName() const { return "SymbolAddress"; }

  bool isSymbolReference() const { return true; }
  symbol::ReferencePtr asSymbolReference() const { return reference; }

  virtual bool isWriteable() { return true; }
  virtual bool isReadable() { return true; }

protected:
  symbol::ReferencePtr reference;
};

}
}

#endif