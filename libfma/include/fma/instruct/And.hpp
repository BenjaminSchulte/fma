#ifndef __FMA_INSTRUCT_AND_H__
#define __FMA_INSTRUCT_AND_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class AND : public assem::Instruction2 {
public:
  AND(assem::Operand *left, assem::Operand *right) : assem::Instruction2(left, right) {}

  std::string getName() { return "AND"; };
};

}
}

#endif