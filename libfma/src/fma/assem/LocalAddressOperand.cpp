#include <fma/assem/LocalAddressOperand.hpp>
#include <sstream>

using namespace FMA::assem;

// ----------------------------------------------------------------------------
std::string LocalAddressOperand::asString() const {
  std::ostringstream os;
  os << "local(" << address->asString() << ")";
  return os.str();
}
