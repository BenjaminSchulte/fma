#include <fma/symbol/SymbolReference.hpp>
#include <fma/plugin/MemoryPluginAdapter.hpp>
#include <sstream>
#include <iostream>

using namespace FMA::symbol;

// ----------------------------------------------------------------------------
uint64_t SymbolReference::resolve(const plugin::MemorySymbolMap *map, bool &valid) const {
  if (!map->hasResolved(name)) {
    valid = false;
    return 0;
  }

  return map->getResolved(name);
}
