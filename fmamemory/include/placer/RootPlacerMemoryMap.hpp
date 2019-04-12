#ifndef __FMA_PLACER_ROOTPLACERMEMORYMAP_H__
#define __FMA_PLACER_ROOTPLACERMEMORYMAP_H__

#include "PlacerMemoryMap.hpp"

namespace FMA {
namespace placer {

class RootPlacerMemoryMap : public PlacerMemoryMap {
public:
  RootPlacerMemoryMap(Placer *placer);

  PlacerMemoryMapBank *getReadMemoryMap(memory::MemoryBankIndex bank);
};

}
}

#endif
