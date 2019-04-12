#ifndef __FMA_INSTRUCT_DEC_H__
#define __FMA_INSTRUCT_DEC_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class DEC : public assem::Instruction1 {
public:
  DEC(assem::Operand *op) : assem::Instruction1(op) {}

  std::string getName() { return "DEC"; };
};

}
}

#endif