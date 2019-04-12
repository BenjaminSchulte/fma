#ifndef __FMA_INSTRUCT_POP_H__
#define __FMA_INSTRUCT_POP_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class POP : public assem::Instruction1 {
public:
  POP(assem::Operand *op) : assem::Instruction1(op) {}

  std::string getName() { return "POP"; };
};

}
}

#endif