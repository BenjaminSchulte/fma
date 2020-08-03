#include <fma/symbol/SignedAssertRangeReference.hpp>
#include <fma/symbol/ConstantNumber.hpp>
#include <fma/output/DynamicBuffer.hpp>
#include <sstream>

using namespace FMA::symbol;

// ----------------------------------------------------------------------------
std::string SignedAssertRangeReference::asString() const {
  std::ostringstream os;
  os << other->asString();
  return os.str();
}

// ----------------------------------------------------------------------------
uint64_t SignedAssertRangeReference::resolve(const plugin::MemorySymbolMap *map, bool &valid) const {
  int64_t number = other->resolve(map, valid);
  if (!valid) {
    return 0;
  }

  if (number < min || number > max) {
    valid = false;
  }

  return number;
}

// ----------------------------------------------------------------------------
bool SignedAssertRangeReference::serialize(FMA::output::DynamicBuffer &buffer) const {
  uint16_t id = (uint16_t)SerializeReferenceId::SIGNED_ASSET_RANGE_REFERENCE;
  buffer.write(&id, 2);

  ConstantNumber(min).serialize(buffer);
  ConstantNumber(max).serialize(buffer);

  return true;
}