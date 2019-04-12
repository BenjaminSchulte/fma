#include <fma/assem/ConstantNumberOperand.hpp>
#include <fma/symbol/ConstantNumber.hpp>
#include <sstream>

using namespace FMA::assem;
using namespace FMA::symbol;

// ----------------------------------------------------------------------------
std::string ConstantNumberOperand::asString() const {
  std::ostringstream os;
  os << number;
  return os.str();
}

// ----------------------------------------------------------------------------
ReferencePtr ConstantNumberOperand::asSymbolReference() const {
  return ReferencePtr(new ConstantNumber(number));
}

// ----------------------------------------------------------------------------
