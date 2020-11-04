#ifndef __FMA65816_LANG_MOVEBLOCK_H__
#define __FMA65816_LANG_MOVEBLOCK_H__

#include <fma/assem/Instruction.hpp>

namespace FMA65816 {
namespace lang {

class MoveBlock : public FMA::assem::Instruction2 {
  public:
  MoveBlock(bool isPositive, FMA::assem::Operand *srcBank, FMA::assem::Operand *dstBank)
    : FMA::assem::Instruction2(srcBank, dstBank)
    , mIsPositive(isPositive)
  {}

  std::string getName() { return mIsPositive ? "MVP" : "MVN"; };

protected:
  bool mIsPositive;
};

}
}

#endif
