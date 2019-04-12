#ifndef __FMA_INSTRUCT_NOT_H__
#define __FMA_INSTRUCT_NOT_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class NOT : public assem::Instruction2 {
public:
  NOT(assem::Operand *left, assem::Operand *right) : assem::Instruction2(left, right) {}

  std::string getName() { return "NOT"; };
};

}
}

#endif