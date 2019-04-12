#ifndef __FMA_INSTRUCT_JNZ_H__
#define __FMA_INSTRUCT_JNZ_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class JNZ : public assem::Instruction1 {
public:
  JNZ(assem::Operand *op) : assem::Instruction1(op) {}

  std::string getName() { return "JNZ"; };
};

}
}

#endif