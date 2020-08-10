#include <fma/symbol/Reference.hpp>
#include <fma/symbol/SymbolReference.hpp>
#include <fma/symbol/ConstantNumber.hpp>
#include <fma/Log.hpp>
#include <fma/output/DynamicBuffer.hpp>

using namespace FMA::symbol;

// ----------------------------------------------------------------------------
ReferencePtr Reference::deserialize(Log *log, FMA::output::DynamicBuffer &buffer) {
  int type = buffer.readUnsigned(2);

  switch ((SerializeReferenceId)type) {
  case SerializeReferenceId::SYMBOL_REFERENCE:
    return SymbolReference::deserialize(log, buffer);

  case SerializeReferenceId::CONST8_REFERENCE:
    return ConstantNumber::deserialize(log, buffer, 1);

  case SerializeReferenceId::CONST16_REFERENCE:
    return ConstantNumber::deserialize(log, buffer, 2);

  case SerializeReferenceId::CONST32_REFERENCE:
    return ConstantNumber::deserialize(log, buffer, 4);

  case SerializeReferenceId::CONST64_REFERENCE:
    return ConstantNumber::deserialize(log, buffer, 8);

  case SerializeReferenceId::CALCULATE_REFERENCE:
  case SerializeReferenceId::SIGNED_ASSET_RANGE_REFERENCE:
  default:
    log->error() << "Invalid reference type ID " << type;
    break;
  }

  return ReferencePtr();
}
