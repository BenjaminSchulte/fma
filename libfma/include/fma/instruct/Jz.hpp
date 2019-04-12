#ifndef __FMA_INSTRUCT_JZ_H__
#define __FMA_INSTRUCT_JZ_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class JZ : public assem::Instruction1 {
public:
  JZ(assem::Operand *op) : assem::Instruction1(op) {}

  std::string getName() { return "JZ"; };
};

}
}

#endif