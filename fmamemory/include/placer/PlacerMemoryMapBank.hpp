#ifndef __FMA_PLACER_PLACERMEMORYMAPBANK_H__
#define __FMA_PLACER_PLACERMEMORYMAPBANK_H__

#include <memory/Configuration.hpp>
#include <memory/MemoryLocationList.hpp>
#include "UsageMap.hpp"
#include <string>

namespace FMA {
namespace placer {

class PlacerMemoryMap;

class PlacerMemoryMapBank {
public:
  PlacerMemoryMapBank(PlacerMemoryMap *owner, memory::MemoryBankIndex index);
  virtual ~PlacerMemoryMapBank();

  void dump(const std::string &prefix);

  void filterValidLocations(const memory::MemoryLocationConstraint &constraint, memory::MemoryLocationList &result, const memory::MemoryBankSize &size);
  bool block(const memory::MemoryBankSize &offset, const memory::MemoryBankSize &size);

protected:
  PlacerMemoryMap *owner;
  memory::MemoryBankIndex index;

  void addCleanedUpLocations(const memory::MemoryLocationConstraint &constraint, memory::MemoryLocationList &result);
  void addAlignedRangeItem(const memory::MemoryLocationRange &range, const memory::MemoryLocationConstraint &constraint, memory::MemoryLocationList &result);
  void addDefaultRangeItem(const memory::MemoryLocationRange &range, const memory::MemoryLocationConstraint &constraint, memory::MemoryLocationList &result);
};

}
}

#endif
