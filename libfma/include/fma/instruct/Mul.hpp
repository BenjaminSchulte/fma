#ifndef __FMA_INSTRUCT_MUL_H__
#define __FMA_INSTRUCT_MUL_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class MUL : public assem::Instruction2 {
public:
  MUL(assem::Operand *left, assem::Operand *right) : assem::Instruction2(left, right) {}

  std::string getName() { return "MUL"; };
};

}
}

#endif