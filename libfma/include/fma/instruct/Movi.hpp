#ifndef __FMA_INSTRUCT_MOVI_H__
#define __FMA_INSTRUCT_MOVI_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class MOVI : public assem::Instruction2 {
public:
  MOVI(assem::Operand *left, assem::Operand *right) : assem::Instruction2(left, right) {}

  std::string getName() { return "MOVI"; };
};

}
}

#endif