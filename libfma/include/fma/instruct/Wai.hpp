#ifndef __FMA_INSTRUCT_WAI_H__
#define __FMA_INSTRUCT_WAI_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class WAI : public assem::Instruction0 {
public:
  std::string getName() { return "WAI"; };
};

}
}

#endif