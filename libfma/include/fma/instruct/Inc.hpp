#ifndef __FMA_INSTRUCT_INC_H__
#define __FMA_INSTRUCT_INC_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class INC : public assem::Instruction1 {
public:
  INC(assem::Operand *op) : assem::Instruction1(op) {}

  std::string getName() { return "INC"; };
};

}
}

#endif