#ifndef __FMA_INSTRUCT_DIV_H__
#define __FMA_INSTRUCT_DIV_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class DIV : public assem::Instruction2 {
public:
  DIV(assem::Operand *left, assem::Operand *right) : assem::Instruction2(left, right) {}

  std::string getName() { return "DIV"; };
};

}
}

#endif