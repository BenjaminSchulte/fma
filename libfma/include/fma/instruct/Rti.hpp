#ifndef __FMA_INSTRUCT_RTI_H__
#define __FMA_INSTRUCT_RTI_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class RTI : public assem::Instruction0 {
public:
  std::string getName() { return "RTI"; };
};

}
}

#endif