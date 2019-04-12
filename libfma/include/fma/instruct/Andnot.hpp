#ifndef __FMA_INSTRUCT_ANDNOT_H__
#define __FMA_INSTRUCT_ANDNOT_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class ANDNOT : public assem::Instruction2 {
public:
  ANDNOT(assem::Operand *left, assem::Operand *right) : assem::Instruction2(left, right) {}
  
  std::string getName() { return "ANDNOT"; };
};

}
}

#endif