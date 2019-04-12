#include <placer/RootPlacerMemoryMap.hpp>

using namespace FMA::placer;
using namespace FMA::memory;

// ----------------------------------------------------------------------------
RootPlacerMemoryMap::RootPlacerMemoryMap(Placer *placer)
  : PlacerMemoryMap(placer)
{
  name = "$ROOT";
}

// ----------------------------------------------------------------------------
PlacerMemoryMapBank *RootPlacerMemoryMap::getReadMemoryMap(MemoryBankIndex bank) {
  return getOwnMemoryMap(bank);
}
