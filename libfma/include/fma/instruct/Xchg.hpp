#ifndef __FMA_INSTRUCT_XCHG_H__
#define __FMA_INSTRUCT_XCHG_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class XCHG : public assem::Instruction2 {
public:
  XCHG(assem::Operand *left, assem::Operand *right) : assem::Instruction2(left, right) {}

  std::string getName() { return "XCHG"; };
};

}
}

#endif