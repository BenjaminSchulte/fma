#ifndef __FMA_INSTRUCT_JS_H__
#define __FMA_INSTRUCT_JS_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class JS : public assem::Instruction1 {
public:
  JS(assem::Operand *op) : assem::Instruction1(op) {}

  std::string getName() { return "JS"; };
};

}
}

#endif