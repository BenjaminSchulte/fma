#ifndef __FMA_INSTRUCT_JNO_H__
#define __FMA_INSTRUCT_JNO_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class JNO : public assem::Instruction1 {
public:
  JNO(assem::Operand *op) : assem::Instruction1(op) {}

  std::string getName() { return "JNO"; };
};

}
}

#endif