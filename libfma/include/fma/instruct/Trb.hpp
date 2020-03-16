#ifndef __FMA_INSTRUCT_TRB_H__
#define __FMA_INSTRUCT_TRB_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class TRB : public assem::Instruction2 {
public:
  TRB(assem::Operand *left, assem::Operand *right) : assem::Instruction2(left, right) {}

  std::string getName() { return "TRB"; };
};

}
}

#endif