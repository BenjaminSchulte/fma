#include <fma/assem/MemoryAddressOperand.hpp>
#include <sstream>

using namespace FMA::assem;

// ----------------------------------------------------------------------------
std::string MemoryAddressOperand::asString() const {
  std::ostringstream os;
  os << "[" << address->asString() << "]";
  return os.str();
}
