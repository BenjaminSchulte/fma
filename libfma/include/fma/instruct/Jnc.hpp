#ifndef __FMA_INSTRUCT_JNC_H__
#define __FMA_INSTRUCT_JNC_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class JNC : public assem::Instruction1 {
public:
  JNC(assem::Operand *op) : assem::Instruction1(op) {}

  std::string getName() { return "JNC"; };
};

}
}

#endif