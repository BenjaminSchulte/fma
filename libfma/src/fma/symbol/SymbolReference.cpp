#include <fma/symbol/SymbolReference.hpp>
#include <fma/plugin/MemoryPluginAdapter.hpp>
#include <fma/output/DynamicBuffer.hpp>
#include <sstream>
#include <iostream>

using namespace FMA::symbol;

// ----------------------------------------------------------------------------
SymbolReferencePtr SymbolReference::deserialize(Log *, FMA::output::DynamicBuffer &buffer) {
  return SymbolReferencePtr(new SymbolReference(buffer.readString()));
}

// ----------------------------------------------------------------------------
bool SymbolReference::serialize(FMA::output::DynamicBuffer &buffer) const {
  uint16_t id = (uint16_t)SerializeReferenceId::SYMBOL_REFERENCE;
  buffer.write(&id, 2);
  buffer.writeString(name);
  return true;
}

// ----------------------------------------------------------------------------
uint64_t SymbolReference::resolve(const plugin::MemorySymbolMap *map, bool &valid) const {
  if (!map->hasResolved(name)) {
    valid = false;
    return 0;
  }

  return map->getResolved(name);
}
