#include <fma/assem/RelativeAddressOperand.hpp>
#include <fma/symbol/CalculatedNumber.hpp>
#include <iostream>
#include <sstream>

using namespace FMA::assem;
using namespace FMA::symbol;

// ----------------------------------------------------------------------------
std::string RelativeAddressOperand::asString() const {
  std::ostringstream os;
  os << left->asString() << "+" << right->asString();
  return os.str();
}

// ----------------------------------------------------------------------------
bool RelativeAddressOperand::isSymbolReference() const {
  return left->isSymbolReference() || right->isSymbolReference();
}

// ----------------------------------------------------------------------------
ReferencePtr RelativeAddressOperand::asSymbolReference() const {
  if (left->isSymbolReference() && right->isSymbolReference()) {
    return ReferencePtr(new CalculatedNumber(
      left->asSymbolReference(),
      CalculatedNumber::ADD,
      right->asSymbolReference()
    ));
  } else if (left->isSymbolReference()) {
    return left->asSymbolReference();
  } else {
    return right->asSymbolReference();
  }
}

// ----------------------------------------------------------------------------
bool RelativeAddressOperand::isResolvedAddress() const {
  return left->isResolvedAddress() || right->isResolvedAddress();
}

// ----------------------------------------------------------------------------
uint64_t RelativeAddressOperand::asResolvedAddress() const {
  if (left->isResolvedAddress() && right->isResolvedAddress()) {
    return left->asResolvedAddress() + right->asResolvedAddress();
  } else if (left->isResolvedAddress()) {
    return left->asResolvedAddress();
  } else {
    return right->asResolvedAddress();
  }
}
