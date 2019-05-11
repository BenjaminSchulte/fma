#ifndef __FMA_ASM_FLAGOPERAND_H__
#define __FMA_ASM_FLAGOPERAND_H__

#include "Operand.hpp"

namespace FMA {
namespace assem {

class FlagOperand : public Operand {
public:
  FlagOperand(const char *name) : name(name) {};
  virtual ~FlagOperand() {};

  virtual std::string asString() const { return name; }
  virtual std::string getIdentifier() { return name; }

  virtual std::string getTypeName() const { return "Flag"; }

  virtual bool isWriteable() { return true; }
  virtual bool isReadable() { return true; }

protected:
  const char *name;
};

}
}

#endif