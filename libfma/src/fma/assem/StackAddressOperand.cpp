#include <fma/assem/StackAddressOperand.hpp>
#include <sstream>

using namespace FMA::assem;

// ----------------------------------------------------------------------------
std::string StackAddressOperand::asString() const {
  std::ostringstream os;
  os << "stack(" << address->asString() << ")";
  return os.str();
}
