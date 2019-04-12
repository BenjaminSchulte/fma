#ifndef __FMA_INSTRUCT_PUSH_H__
#define __FMA_INSTRUCT_PUSH_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class PUSH : public assem::Instruction1 {
public:
  PUSH(assem::Operand *op) : assem::Instruction1(op) {}

  std::string getName() { return "PUSH"; };
};

}
}

#endif