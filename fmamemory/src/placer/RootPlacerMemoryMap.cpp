#include <placer/RootPlacerMemoryMap.hpp>

using namespace FMA::placer;
using namespace FMA::memory;

// ----------------------------------------------------------------------------
RootPlacerMemoryMap::RootPlacerMemoryMap(Placer *placer)
  : PlacerMemoryMap(placer, false)
{
  name = "$ROOT";
}
