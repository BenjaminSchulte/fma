#ifndef __FMA_INSTRUCT_CMP_H__
#define __FMA_INSTRUCT_CMP_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class CMP : public assem::Instruction2 {
public:
  CMP(assem::Operand *left, assem::Operand *right) : assem::Instruction2(left, right) {}

  std::string getName() { return "CMP"; };
};

}
}

#endif