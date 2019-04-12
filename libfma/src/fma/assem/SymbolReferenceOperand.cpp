#include <fma/assem/SymbolReferenceOperand.hpp>
#include <sstream>

using namespace FMA::assem;

// ----------------------------------------------------------------------------
std::string SymbolReferenceOperand::asString() const {
  std::ostringstream os;
  os << reference->asString();
  return os.str();
}
