#ifndef __FMA_INSTRUCT_BRK_H__
#define __FMA_INSTRUCT_BRK_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class BRK : public assem::Instruction0 {
public:
  std::string getName() { return "BRK"; };
};

}
}

#endif