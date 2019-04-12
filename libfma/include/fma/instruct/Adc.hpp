#ifndef __FMA_INSTRUCT_ADC_H__
#define __FMA_INSTRUCT_ADC_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class ADC : public assem::Instruction2 {
public:
  ADC(assem::Operand *left, assem::Operand *right) : assem::Instruction2(left, right) {}

  std::string getName() { return "ADC"; };
};

}
}

#endif