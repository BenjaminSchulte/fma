#ifndef __FMA_INSTRUCT_NOP_H__
#define __FMA_INSTRUCT_NOP_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class NOP : public assem::Instruction0 {
public:
  std::string getName() { return "NOP"; };
};

}
}

#endif