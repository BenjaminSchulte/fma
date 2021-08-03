#include <placer/Placer.hpp>
#include <placer/PlacerMemoryMap.hpp>
#include <placer/PlacerMemoryMapBank.hpp>
#include <placer/UsageMap.hpp>
#include <placer/MergedUsageMap.hpp>
#include <memory/MemoryMap.hpp>
#include <memory/MemoryMapBank.hpp>
#include <memory/MemoryMapBankSection.hpp>
#include <memory/MemoryMapShadow.hpp>
#include <iostream>

using namespace FMA::placer;
using namespace FMA::memory;

// ----------------------------------------------------------------------------
PlacerMemoryMap::~PlacerMemoryMap() {
  removeAllBanks();
}

// ----------------------------------------------------------------------------
void PlacerMemoryMap::removeAllBanks() {
  for (auto &bank : ownBanks) {
    PlacerMemoryMapBank *item = bank.second;
    delete item;
  }
  for (auto &map : usageMaps) {
    delete map.second;
  }
  for (auto &map : mergedUsageMaps) {
    delete map.second;
  }
  ownBanks.clear();
  usageMaps.clear();
  mergedUsageMaps.clear();
}

// ----------------------------------------------------------------------------
void PlacerMemoryMap::addChild(const PlacerMemoryMapPtr &child) {
  children.push_back(child);
  child->addParentWithoutChild(shared_from_this());
}

// ----------------------------------------------------------------------------
void PlacerMemoryMap::addParentWithoutChild(const WeakPlacerMemoryMapPtr &parent) {
  parents.push_back(parent);
}

// ----------------------------------------------------------------------------
UsageMap *PlacerMemoryMap::createUsageMap(memory::MemoryBankIndex index) {
  MemoryMapBank *bank = placer->getMemoryMap()->getBank(index);
  if (bank == NULL) {
    return new UsageMap(0, 0);
  }

  UsageMap *usage = new UsageMap(bank->getOffset(), bank->getSize(), UsageMapSection::BLOCKED);
  for (auto &section : bank->getSections()) {
    usage->free(section->getOffset(), section->getSize());
  }

  return usage;
}

// ----------------------------------------------------------------------------
bool PlacerMemoryMap::isShared() const {
  return mShared;
}

// ----------------------------------------------------------------------------
bool PlacerMemoryMap::isParentShared() const {
  for (auto &_parent : parents) {
    auto parent(_parent.lock());
    if (parent && parent->isShared()) {
      return true;
    }
  }

  return false;
}

// ----------------------------------------------------------------------------
bool PlacerMemoryMap::needsOwnUsageMap() const {
  return isShared() || isParentShared() || parents.empty() || parents.size() > 1;
}

// ----------------------------------------------------------------------------
PlacerMemoryMap *PlacerMemoryMap::closestMemoryMapOwner() {
  PlacerMemoryMap *result = closestMemoryMapOwnerOrNull(true);
  if (result) {
    return result;
  }

  return this;
}

// ----------------------------------------------------------------------------
PlacerMemoryMap *PlacerMemoryMap::closestMemoryMapOwnerOrNull(bool includeSelf) {
  if (includeSelf && needsOwnUsageMap()) {
    return this;
  }

  for (const auto &_parent : parents) {
    auto parent = _parent.lock();
    if (parent) {
      PlacerMemoryMap *result = parent->closestMemoryMapOwner();
      if (result) {
        return result;
      }
    }
  }

  return nullptr;
}

// ----------------------------------------------------------------------------
MergedUsageMap *PlacerMemoryMap::getUsageMap(memory::MemoryBankIndex bank) {
  // Returns the cached result, if present
  MergedUsageMapList::iterator it(mergedUsageMaps.find(bank));
  if (it != mergedUsageMaps.end()) {
    return it->second;
  }

  // Checks whether we should have a usage map
  PlacerMemoryMap *closest = closestMemoryMapOwner();
  if (closest == this) {
    UsageMap *map = createUsageMap(bank);
    usageMaps[bank] = map;

    MergedUsageMap *merged = new MergedUsageMap(this, map, bank);
    mergedUsageMaps[bank] = merged;
    return merged;
  }

  // Returns the parent usage map
  return closest->getUsageMap(bank);
}

// ----------------------------------------------------------------------------
PlacerMemoryMapBank *PlacerMemoryMap::getMemoryMap(MemoryBankIndex bank) {
  PlacerMemoryMapBankList::const_iterator it = ownBanks.find(bank);
  
  if (it == ownBanks.end()) {
    PlacerMemoryMapBank *item = createOwnMemoryMap(bank);
    ownBanks[bank] = item;
    return item;
  } else {
    return it->second;
  }
}

// ----------------------------------------------------------------------------
PlacerMemoryMapBank *PlacerMemoryMap::createOwnMemoryMap(MemoryBankIndex bank) {
  return new PlacerMemoryMapBank(this, bank);
}

// ----------------------------------------------------------------------------
bool PlacerMemoryMap::place(const memory::MemoryLocationConstraint &location, const memory::MemoryBankSize &size) {
  if (!location.banks().size() || !location.addresses().size()) {
    return false;
  }

  return place(location.banks().front(), location.addresses().front(), size);
}

// ----------------------------------------------------------------------------
bool PlacerMemoryMap::place(const memory::MemoryBankIndex bankIndex, const memory::MemoryBankSize &offset, const memory::MemoryBankSize &size) {
  MemoryMapBank *bank = placer->getMemoryMap()->getBank(bankIndex);
  if (bank == NULL) {
    return false;
  }

  for (auto &shadow : bank->getShadowsAt(offset)) {
    int64_t diff = (int64_t)shadow->getOtherAddress(bank) - (int64_t)shadow->getOwnAddress(bank);

    if (!placeWithoutShadowTest(shadow->getOtherBank(bank)->getIndex(), offset + diff, size)) {
      return false;
    }
  }

  return placeWithoutShadowTest(bankIndex, offset, size);
}

// ----------------------------------------------------------------------------
bool PlacerMemoryMap::placeWithoutShadowTest(const memory::MemoryBankIndex bankIndex, const memory::MemoryBankSize &offset, const memory::MemoryBankSize &size) {
  PlacerMemoryMapBank *bank = getMemoryMap(bankIndex);
  return bank->block(offset, size);
}

// ----------------------------------------------------------------------------
void PlacerMemoryMap::filterValidLocations(const MemoryLocationConstraint &constraint, MemoryLocationList &result, const MemoryBankSize &size) {
  const auto &banks = constraint.banks();
  if (!banks.size()) {
    for (const auto &pair : placer->getMemoryMap()->getBanks()) {
      if (pair.second->isPureShadowBank()) {
        continue;
      }

      getMemoryMap(pair.first)->filterValidLocations(constraint, result, size);
    }
    return;
  }

  for (const auto &bank : banks) {
    getMemoryMap(bank)->filterValidLocations(constraint, result, size);
  }
}

// ----------------------------------------------------------------------------
MemoryLocationList PlacerMemoryMap::filterValidLocations(const MemoryLocationList &locations, const MemoryBankSize &size) {
  MemoryLocationList result;

  for (const auto &location : locations.getAllowed()) {
    filterValidLocations(location, result, size);
  }

  return result;
}

// ----------------------------------------------------------------------------
void PlacerMemoryMap::dump() const {
  dump("");
}

// ----------------------------------------------------------------------------
void PlacerMemoryMap::dump(const std::string &prefix) const {
  std::cout << prefix << "MemoryMap : " << name << std::endl;

  for (const auto &pair : ownBanks) {
    pair.second->dump(prefix + ".. ");
  }

  for (const auto &weakParent : parents) {
    const auto &parent = weakParent.lock();
    if (parent) {
      parent->dump(prefix + "++ ");
    }
  }
}
