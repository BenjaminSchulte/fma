#ifndef __FMA_INSTRUCT_JBS_H__
#define __FMA_INSTRUCT_JBS_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class JBS : public assem::Instruction3 {
public:
  JBS(assem::Operand *left, assem::Operand *mid, assem::Operand *right) : assem::Instruction3(left, mid, right) {}

  std::string getName() { return "JBS"; };
};

}
}

#endif