#ifndef __FMA_INSTRUCT_ROR_H__
#define __FMA_INSTRUCT_ROR_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class ROR : public assem::Instruction2 {
public:
  ROR(assem::Operand *left, assem::Operand *right) : assem::Instruction2(left, right) {}

  std::string getName() { return "ROR"; };
};

}
}

#endif