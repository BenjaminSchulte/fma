#ifndef __FMA_INSTRUCT_JNS_H__
#define __FMA_INSTRUCT_JNS_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class JNS : public assem::Instruction1 {
public:
  JNS(assem::Operand *op) : assem::Instruction1(op) {}

  std::string getName() { return "JNS"; };
};

}
}

#endif