#ifndef __FMA_INSTRUCT_ROL_H__
#define __FMA_INSTRUCT_ROL_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class ROL : public assem::Instruction2 {
public:
  ROL(assem::Operand *left, assem::Operand *right) : assem::Instruction2(left, right) {}

  std::string getName() { return "ROL"; };
};

}
}

#endif