#ifndef __FMA_INSTRUCT_ASL_H__
#define __FMA_INSTRUCT_ASL_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class ASL : public assem::Instruction2 {
public:
  ASL(assem::Operand *left, assem::Operand *right) : assem::Instruction2(left, right) {}

  std::string getName() { return "ASL"; };
};

}
}

#endif