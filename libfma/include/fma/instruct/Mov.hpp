#ifndef __FMA_INSTRUCT_MOV_H__
#define __FMA_INSTRUCT_MOV_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class MOV : public assem::Instruction2 {
public:
  MOV(assem::Operand *left, assem::Operand *right) : assem::Instruction2(left, right) {}

  std::string getName() { return "MOV"; };
};

}
}

#endif