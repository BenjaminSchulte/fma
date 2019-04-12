#ifndef __FMA_INSTRUCT_MOVW_H__
#define __FMA_INSTRUCT_MOVW_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class MOVW : public assem::Instruction2 {
public:
  MOVW(assem::Operand *left, assem::Operand *right) : assem::Instruction2(left, right) {}

  std::string getName() { return "MOVW"; };
};

}
}

#endif