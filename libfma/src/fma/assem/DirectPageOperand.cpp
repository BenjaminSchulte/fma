#include <fma/assem/DirectPageOperand.hpp>
#include <sstream>

using namespace FMA::assem;

// ----------------------------------------------------------------------------
std::string DirectPageOperand::asString() const {
  std::ostringstream os;
  os << "dp(" << address->asString() << ")";
  return os.str();
}
