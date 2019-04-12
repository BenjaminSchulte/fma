#include <placer/Placer.hpp>
#include <placer/PlacerMemoryMap.hpp>
#include <placer/PlacerMemoryMapBank.hpp>
#include <placer/UsageMap.hpp>
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
  ownBanks.clear();
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

  updateUsageMap(index, usage);

  return usage;
}

// ----------------------------------------------------------------------------
void PlacerMemoryMap::updateUsageMap(memory::MemoryBankIndex, UsageMap *) {

}

// ----------------------------------------------------------------------------
PlacerMemoryMapBank *PlacerMemoryMap::getOwnMemoryMap(MemoryBankIndex bank) {
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
PlacerMemoryMapBank *PlacerMemoryMap::getWriteMemoryMap(MemoryBankIndex bank) {
  PlacerMemoryMapBankList::const_iterator it = ownBanks.find(bank);
  if (it != ownBanks.end()) {
    return it->second;
  }

  if (parents.size() == 1) {
    auto parent = parents.front().lock();
    if (parent) {
      return parent->getWriteMemoryMap(bank);
    }
  }

  return getOwnMemoryMap(bank);
}

// ----------------------------------------------------------------------------
PlacerMemoryMapBank *PlacerMemoryMap::getReadMemoryMap(MemoryBankIndex bank) {
  PlacerMemoryMapBankList::const_iterator it = ownBanks.find(bank);
  if (it != ownBanks.end()) {
    return it->second;
  }

  if (parents.size() == 1) {
    auto parent = parents.front().lock();
    if (parent) {
      return parent->getReadMemoryMap(bank);
    }
  }

  return getOwnMemoryMap(bank);
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
  PlacerMemoryMapBank *bank = getWriteMemoryMap(bankIndex);
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

      getReadMemoryMap(pair.first)->filterValidLocations(constraint, result, size);
    }
    return;
  }

  for (const auto &bank : banks) {
    getReadMemoryMap(bank)->filterValidLocations(constraint, result, size);
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
