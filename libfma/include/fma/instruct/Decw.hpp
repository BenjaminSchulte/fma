#ifndef __FMA_INSTRUCT_DECW_H__
#define __FMA_INSTRUCT_DECW_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class DECW : public assem::Instruction1 {
public:
  DECW(assem::Operand *op) : assem::Instruction1(op) {}

  std::string getName() { return "DECW"; };
};

}
}

#endif