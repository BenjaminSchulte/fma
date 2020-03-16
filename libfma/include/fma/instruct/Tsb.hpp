#ifndef __FMA_INSTRUCT_TSB_H__
#define __FMA_INSTRUCT_TSB_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class TSB : public assem::Instruction2 {
public:
  TSB(assem::Operand *left, assem::Operand *right) : assem::Instruction2(left, right) {}

  std::string getName() { return "TSB"; };
};

}
}

#endif