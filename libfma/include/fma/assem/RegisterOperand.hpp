#ifndef __FMA_ASM_REGISTEROPERAND_H__
#define __FMA_ASM_REGISTEROPERAND_H__

#include "Operand.hpp"

namespace FMA {
namespace assem {

class RegisterOperand : public Operand {
public:
  RegisterOperand(const char *name) : name(name) {};
  virtual ~RegisterOperand() {};

  virtual std::string asString() const { return name; }
  virtual std::string getIdentifier() { return name; }

  virtual std::string getTypeName() const { return "Register"; }

  virtual bool isWriteable() { return true; }
  virtual bool isReadable() { return true; }

protected:
  const char *name;
};

}
}

#endif