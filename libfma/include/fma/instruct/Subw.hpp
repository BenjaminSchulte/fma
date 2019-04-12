#ifndef __FMA_INSTRUCT_SUBW_H__
#define __FMA_INSTRUCT_SUBW_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class SUBW : public assem::Instruction2 {
public:
  SUBW(assem::Operand *left, assem::Operand *right) : assem::Instruction2(left, right) {}

  std::string getName() { return "SUBW"; };
};

}
}

#endif