#ifndef __FMA_INSTRUCT_RTL_H__
#define __FMA_INSTRUCT_RTL_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class RTL : public assem::Instruction0 {
public:
  std::string getName() { return "RTL"; };
};

}
}

#endif