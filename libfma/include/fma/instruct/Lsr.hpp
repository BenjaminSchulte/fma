#ifndef __FMA_INSTRUCT_LSR_H__
#define __FMA_INSTRUCT_LSR_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class LSR : public assem::Instruction2 {
public:
  LSR(assem::Operand *left, assem::Operand *right) : assem::Instruction2(left, right) {}

  std::string getName() { return "LSR"; };
};

}
}

#endif