#ifndef __FMA_INSTRUCT_INCW_H__
#define __FMA_INSTRUCT_INCW_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class INCW : public assem::Instruction1 {
public:
  INCW(assem::Operand *op) : assem::Instruction1(op) {}

  std::string getName() { return "INCW"; };
};

}
}

#endif