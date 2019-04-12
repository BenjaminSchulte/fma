#ifndef __FMA_INSTRUCT_JMP_H__
#define __FMA_INSTRUCT_JMP_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class JMP : public assem::Instruction1 {
public:
  JMP(assem::Operand *op) : assem::Instruction1(op) {}

  std::string getName() { return "JMP"; };
};

}
}

#endif