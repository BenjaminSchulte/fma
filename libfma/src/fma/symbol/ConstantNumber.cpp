#include <fma/symbol/ConstantNumber.hpp>
#include <sstream>

using namespace FMA::symbol;

// ----------------------------------------------------------------------------
std::string ConstantNumber::asString() const {
  std::ostringstream os;
  os << number;
  return os.str();
}

// ----------------------------------------------------------------------------
uint64_t ConstantNumber::resolve(const plugin::MemorySymbolMap *, bool &) const {
  return number;
}
