#ifndef __FMA_INSTRUCT_JGE_H__
#define __FMA_INSTRUCT_JGE_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class JGE : public assem::Instruction1 {
public:
  JGE(assem::Operand *op) : assem::Instruction1(op) {}

  std::string getName() { return "JGE"; };
};

}
}

#endif