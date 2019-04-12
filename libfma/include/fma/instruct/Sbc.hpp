#ifndef __FMA_INSTRUCT_SBC_H__
#define __FMA_INSTRUCT_SBC_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class SBC : public assem::Instruction2 {
public:
  SBC(assem::Operand *left, assem::Operand *right) : assem::Instruction2(left, right) {}

  std::string getName() { return "SBC"; };
};

}
}

#endif