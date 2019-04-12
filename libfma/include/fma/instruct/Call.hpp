#ifndef __FMA_INSTRUCT_CALL_H__
#define __FMA_INSTRUCT_CALL_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class CALL : public assem::Instruction1 {
public:
  CALL(assem::Operand *op) : assem::Instruction1(op) {}

  std::string getName() { return "CALL"; };
};

}
}

#endif