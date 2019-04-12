#include <fma/assem/BitMaskOperand.hpp>
#include <fma/assem/ConstantNumberOperand.hpp>
#include <sstream>

using namespace FMA::assem;
using namespace FMA::symbol;

// ----------------------------------------------------------------------------
BitMaskOperand::BitMaskOperand(Operand *address, uint8_t lsh, uint8_t rsh, uint64_t mask)
  : address(address)
  , lsh(new ConstantNumberOperand(lsh))
  , rsh(new ConstantNumberOperand(rsh))
  , mask(mask)
{
}

// ----------------------------------------------------------------------------
BitMaskOperand::BitMaskOperand(Operand *address, Operand *lsh, Operand *rsh, uint64_t mask) 
  : address(address)
  , lsh(lsh)
  , rsh(rsh)
  , mask(mask)
{
}

// ----------------------------------------------------------------------------
std::string BitMaskOperand::asString() const {
  std::ostringstream os;
  os << address->asString() << "<<" << lsh->asString() << ">>" << rsh->asString() << "&" << mask;
  return os.str();
}

// ----------------------------------------------------------------------------
