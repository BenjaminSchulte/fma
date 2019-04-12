#include <fma/assem/PcRelativeAddressOperand.hpp>
#include <sstream>

using namespace FMA::assem;

// ----------------------------------------------------------------------------
std::string PcRelativeAddressOperand::asString() const {
  std::ostringstream os;
  os << "pcrel(" << address->asString() << ")";
  return os.str();
}
