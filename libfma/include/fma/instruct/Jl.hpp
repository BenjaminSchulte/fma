#ifndef __FMA_INSTRUCT_JL_H__
#define __FMA_INSTRUCT_JL_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class JL : public assem::Instruction1 {
public:
  JL(assem::Operand *op) : assem::Instruction1(op) {}

  std::string getName() { return "JL"; };
};

}
}

#endif