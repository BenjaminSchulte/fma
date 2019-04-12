#ifndef __FMA_INSTRUCT_OR_H__
#define __FMA_INSTRUCT_OR_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class OR : public assem::Instruction2 {
public:
  OR(assem::Operand *left, assem::Operand *right) : assem::Instruction2(left, right) {}

  std::string getName() { return "OR"; };
};

}
}

#endif