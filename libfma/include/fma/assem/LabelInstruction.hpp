#ifndef __FMA_ASM_LABELINSTRUCTION_H__
#define __FMA_ASM_LABELINSTRUCTION_H__

#include <memory>
#include "Instruction.hpp"

namespace FMA {
namespace assem {

class LabelInstruction : public Instruction0 {
public:
  LabelInstruction(const std::string &name) : name(name) {}
  virtual ~LabelInstruction() {}

  virtual std::string getName() { return ".LABEL"; }

  virtual const std::string getSymbol() const { return name; }

protected:
  std::string name;
};

}
}

#endif