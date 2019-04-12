#ifndef __FMA_INSTRUCT_JO_H__
#define __FMA_INSTRUCT_JO_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class JO : public assem::Instruction1 {
public:
  JO(assem::Operand *op) : assem::Instruction1(op) {}

  std::string getName() { return "JO"; };
};

}
}

#endif