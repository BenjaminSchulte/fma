#include <placer/MergedUsageMap.hpp>
#include <placer/PlacerMemoryMap.hpp>
#include <iostream>

using namespace FMA::placer;

namespace {
  uint64_t nextChangeIdentification = 0;
}


// ----------------------------------------------------------------------------
MergedUsageMap::MergedUsageMap(PlacerMemoryMap *owner, UsageMap *map, const memory::MemoryBankIndex bank)
  : mOwner(owner)
  , mLocalMap(map)
  , mIndex(bank)
  , mChangeIdentification(nextChangeIdentification++)
{
}

// ----------------------------------------------------------------------------
MergedUsageMap::~MergedUsageMap() {
  delete mMergeMap;
  delete mParentsMap;
}


// ----------------------------------------------------------------------------
bool MergedUsageMap::isShared() const {
  return mOwner->isShared();
}

// ----------------------------------------------------------------------------
bool MergedUsageMap::block(const uint64_t &offset, const uint64_t &size) {
  if (!mLocalMap->block(offset, size)) {
    return false;
  }

  blockAllParents(offset, size);
  mChangeIdentification = nextChangeIdentification++;
  mMergeMap = nullptr;

  return true;
}

// ----------------------------------------------------------------------------
void MergedUsageMap::blockAllParents(const uint64_t &offset, const uint64_t &size) {
  for (const auto &_parent : mOwner->getParents()) {
    auto parent(_parent.lock());

    if (parent) {
      parent->getUsageMap(mIndex)->block(offset, size);
    }
  }
}

// ----------------------------------------------------------------------------
UsageMapAddressRanges MergedUsageMap::findFreeAddresses(const uint64_t &from, const uint64_t &to, const uint64_t &size) {
  return getMergedUsageMap()->findFreeAddresses(from, to, size);
}

// ----------------------------------------------------------------------------
uint64_t MergedUsageMap::getLatestChangeIdentification() const {
  uint64_t latest = mChangeIdentification;

  for (const auto &_parent : mOwner->getParents()) {
    auto parent(_parent.lock());

    if (parent) {
      uint64_t parentLatest = parent->getUsageMap(mIndex)->getLatestChangeIdentification();

      if (parentLatest > latest) {
        latest = parentLatest;
      }
    }
  }
  return latest;
}

// ----------------------------------------------------------------------------
bool MergedUsageMap::parentHasBeenChanged() const {
  for (const auto &_parent : mOwner->getParents()) {
    auto parent(_parent.lock());

    if (parent) {
      uint64_t parentLatest = parent->getUsageMap(mIndex)->getLatestChangeIdentification();

      if (parentLatest > mParentIdentification) {
        return true;
      }
    }
  }
  return false;
}

// ----------------------------------------------------------------------------
UsageMap *MergedUsageMap::getMergedUsageMap() {
  // Updates the parents map
  delete mParentsMap;
  mParentsMap = nullptr;

  for (const auto &_parent : mOwner->getParents()) {
    auto parent(_parent.lock());
    if (!parent) {
      continue;
    }

    UsageMap *map = parent->getUsageMap(mIndex)->getMergedUsageMap();
    if (mParentsMap == nullptr) {
      mParentsMap = UsageMap::clone(map);
    } else {
      mParentsMap->block(*map);
    }
  }

  delete mMergeMap;
  
  if (mParentsMap) {
    mMergeMap = UsageMap::clone(mParentsMap);
  } else {
    mMergeMap = UsageMap::clone(mLocalMap);
  }
  
  if (isShared()) {
    mMergeMap->set(*mLocalMap, UsageMapSection::BLOCKED, UsageMapSection::FREE);
  } else {
    mMergeMap->block(*mLocalMap);
  }

  mParentIdentification = getLatestChangeIdentification();

  return mMergeMap;
}

