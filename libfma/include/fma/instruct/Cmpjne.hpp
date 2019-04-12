#ifndef __FMA_INSTRUCT_CMPJNE_H__
#define __FMA_INSTRUCT_CMPJNE_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class CMPJNE : public assem::Instruction3 {
public:
  CMPJNE(assem::Operand *left, assem::Operand *mid, assem::Operand *right) : assem::Instruction3(left, mid, right) {}

  std::string getName() { return "CMPJNE"; };
};

}
}

#endif