#ifndef __FMA_ASM_CONSTANTNUMBEROPERAND_H__
#define __FMA_ASM_CONSTANTNUMBEROPERAND_H__

#include "Operand.hpp"

namespace FMA {
namespace assem {

class ConstantNumberOperand : public Operand {
public:
  ConstantNumberOperand(const uint64_t &number) : number(number) {};
  virtual ~ConstantNumberOperand() {};

  virtual std::string asString() const;
  virtual std::string getIdentifier() { return "#"; }

  virtual std::string getTypeName() const { return "ConstantNumber"; }

  virtual bool isConstant() const { return true; }
  virtual uint64_t asConstant() const { return number; }

  virtual bool isResolvedAddress() const { return true; }
  virtual uint64_t asResolvedAddress() const { return number; }

  virtual bool isSymbolReference() const { return true; }
  virtual symbol::ReferencePtr asSymbolReference() const;

  virtual bool isWriteable() { return false; }
  virtual bool isReadable() { return true; }

protected:
  uint64_t number;
};

}
}

#endif