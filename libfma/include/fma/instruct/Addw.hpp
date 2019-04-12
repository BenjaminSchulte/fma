#ifndef __FMA_INSTRUCT_ADDW_H__
#define __FMA_INSTRUCT_ADDW_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class ADDW : public assem::Instruction2 {
public:
  ADDW(assem::Operand *left, assem::Operand *right) : assem::Instruction2(left, right) {}

  std::string getName() { return "ADDW"; };
};

}
}

#endif