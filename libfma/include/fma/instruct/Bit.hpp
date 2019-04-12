#ifndef __FMA_INSTRUCT_BIT_H__
#define __FMA_INSTRUCT_BIT_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class BIT : public assem::Instruction2 {
public:
  BIT(assem::Operand *left, assem::Operand *right) : assem::Instruction2(left, right) {}
  
  std::string getName() { return "BIT"; };
};

}
}

#endif