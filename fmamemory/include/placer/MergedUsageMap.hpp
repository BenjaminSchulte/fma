#pragma once

#include "UsageMap.hpp"
#include <memory/MemoryLocationList.hpp>

namespace FMA {
namespace placer {

class PlacerMemoryMap;

class MergedUsageMap {
public:
  //! Constructor
  MergedUsageMap(PlacerMemoryMap *owner, UsageMap *map, const memory::MemoryBankIndex bank);

  //! Deconstructor
  ~MergedUsageMap();

  //! Blocks an address range
  bool block(const uint64_t &offset, const uint64_t &size);

  //! Finds all free addressed
  UsageMapAddressRanges findFreeAddresses(const uint64_t &from, const uint64_t &to, const uint64_t &size);

  //! Returns the offset
  inline uint64_t getOffset() const { return mLocalMap->getOffset(); }

  //! Returns the size
  inline uint64_t getSize() const { return mLocalMap->getSize(); }

  //! Dumps the usage map
  inline void dump(const std::string &str) const { mLocalMap->dump(str); }

  //! Returns whether the owner node is shared
  bool isShared() const;

  //! Returns the merged usage map
  UsageMap *getMergedUsageMap();

private:
  //! Blocks an address range
  void blockAllParents(const uint64_t &offset, const uint64_t &size);

  //! Checks whether a parent has been changed
  bool parentHasBeenChanged() const;

  //! Checks whether a parent has been changed
  uint64_t getLatestChangeIdentification() const;

  //! The owner
  PlacerMemoryMap *mOwner;

  //! The usage map
  UsageMap *mLocalMap;

  //! The index
  memory::MemoryBankIndex mIndex;

  //! The merged map
  UsageMap *mMergeMap = nullptr;

  //! The parents map
  UsageMap *mParentsMap = nullptr;

  //! The global change identification number
  uint64_t mParentIdentification = 0;

  //! The global change identification number
  uint64_t mChangeIdentification;
};

}
}