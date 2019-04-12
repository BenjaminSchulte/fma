#ifndef __FMA_INSTRUCT_JC_H__
#define __FMA_INSTRUCT_JC_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class JC : public assem::Instruction1 {
public:
  JC(assem::Operand *op) : assem::Instruction1(op) {}

  std::string getName() { return "JC"; };
};

}
}

#endif