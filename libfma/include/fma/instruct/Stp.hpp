#ifndef __FMA_INSTRUCT_STP_H__
#define __FMA_INSTRUCT_STP_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class STP : public assem::Instruction0 {
public:
  std::string getName() { return "STP"; };
};

}
}

#endif