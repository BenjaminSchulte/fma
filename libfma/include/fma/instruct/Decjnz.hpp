#ifndef __FMA_INSTRUCT_DECJNZ_H__
#define __FMA_INSTRUCT_DECJNZ_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class DECJNZ : public assem::Instruction2 {
public:
  DECJNZ(assem::Operand *left, assem::Operand *right) : assem::Instruction2(left, right) {}

  std::string getName() { return "DECJNZ"; };
};

}
}

#endif