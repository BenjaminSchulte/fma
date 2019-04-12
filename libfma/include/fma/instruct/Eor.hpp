#ifndef __FMA_INSTRUCT_EOR_H__
#define __FMA_INSTRUCT_EOR_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class EOR : public assem::Instruction2 {
public:
  EOR(assem::Operand *left, assem::Operand *right) : assem::Instruction2(left, right) {}

  std::string getName() { return "EOR"; };
};

}
}

#endif