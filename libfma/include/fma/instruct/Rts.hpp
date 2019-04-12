#ifndef __FMA_INSTRUCT_RTS_H__
#define __FMA_INSTRUCT_RTS_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class RTS : public assem::Instruction0 {
public:
  std::string getName() { return "RTS"; };
};

}
}

#endif