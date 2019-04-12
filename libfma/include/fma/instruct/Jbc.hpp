#ifndef __FMA_INSTRUCT_JBC_H__
#define __FMA_INSTRUCT_JBC_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class JBC : public assem::Instruction3 {
public:
  JBC(assem::Operand *left, assem::Operand *mid, assem::Operand *right) : assem::Instruction3(left, mid, right) {}

  std::string getName() { return "JBC"; };
};

}
}

#endif