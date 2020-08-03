#include <fma/symbol/ConstantNumber.hpp>
#include <fma/output/DynamicBuffer.hpp>
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

// ----------------------------------------------------------------------------
bool ConstantNumber::serialize(FMA::output::DynamicBuffer &buffer) const {
  uint8_t n8 = number;
  uint16_t n16 = number;
  uint32_t n32 = number;
  uint64_t n64 = number;
  
  if (n8 == n64) {
    uint16_t id = (uint16_t)SerializeReferenceId::CONST8_REFERENCE;
    buffer.write(&id, 2);
    buffer.write(&n8, 1);
  } else if (n16 == n64) {
    uint16_t id = (uint16_t)SerializeReferenceId::CONST16_REFERENCE;
    buffer.write(&id, 2);
    buffer.write(&n16, 2);
  } else if (n32 == n64) {
    uint16_t id = (uint16_t)SerializeReferenceId::CONST32_REFERENCE;
    buffer.write(&id, 2);
    buffer.write(&n32, 4);
  } else {
    uint16_t id = (uint16_t)SerializeReferenceId::CONST64_REFERENCE;
    buffer.write(&id, 2);
    buffer.write(&n64, 8);
  }

  return true;
}