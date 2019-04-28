#include <boost/io/ios_state.hpp>
#include <placer/PlacerMemoryMap.hpp>
#include <placer/PlacerMemoryMapBank.hpp>
#include <memory/MemoryLocationList.hpp>
#include <memory/MemoryLocationConstraint.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace FMA::placer;
using namespace FMA::memory;

// ----------------------------------------------------------------------------
PlacerMemoryMapBank::PlacerMemoryMapBank(PlacerMemoryMap *owner, memory::MemoryBankIndex index)
  : owner(owner)
  , index(index)
  , valid(false)
  , usageMap(NULL)
{
}

// ----------------------------------------------------------------------------
PlacerMemoryMapBank::~PlacerMemoryMapBank() {
  if (usageMap) {
    delete usageMap;
  }
}

// ----------------------------------------------------------------------------
void PlacerMemoryMapBank::update() {
  if (valid) {
    return;
  }

  if (usageMap == NULL) {
    usageMap = owner->createUsageMap(index);
  } else {
    owner->updateUsageMap(index, usageMap);
  }

  valid = true;
}

// ----------------------------------------------------------------------------
UsageMap *PlacerMemoryMapBank::getUsageMap() {
  update();
  return usageMap;
}

// ----------------------------------------------------------------------------
void PlacerMemoryMapBank::invalidate() {
  valid = false;
  //std::cout << "TODO INVALIDATE" << std::endl;
}

// ----------------------------------------------------------------------------
bool PlacerMemoryMapBank::block(const MemoryBankSize &offset, const MemoryBankSize &size) {
  update();
  invalidate();

  return usageMap->block(offset, size);
}

// ----------------------------------------------------------------------------
void PlacerMemoryMapBank::filterValidLocations(const MemoryLocationConstraint &constraint, MemoryLocationList &result, const MemoryBankSize &size) {
  update();

  if (!size) {
    return;
  }

  MemoryLocationConstraint copy(constraint);
  if (constraint.addresses().size()) {
    copy.anyAddress();

    for (const auto &address : constraint.addresses()) {
      bool isValidRange = constraint.ranges().size() > 0 ? false : true;

      if (!isValidRange) {
        std::cout << "XXXX TEST ADDRESS RANGES " << address << std::endl;
        //for (const auto &range : constraint.ranges()) {
        //}
      }

      if (isValidRange) {
        for (auto &block : usageMap->findFreeAddresses(address, address + size - 1, size)) {
          (void)block;
          copy.address(address);
        }
      }
    }

    if (!copy.addresses().size()) {
      return;
    }
  } else if (constraint.ranges().size()) {
    copy.anyRange();

    for (MemoryLocationRange range : constraint.ranges()) {
      for (auto &block : usageMap->findFreeAddresses(range.from, range.to, size)) {
        copy.range(block.from, block.to);
      }
    }

    // No valid range left!
    if (!copy.ranges().size()) {
      return;
    }
  } else {
    for (auto &block : usageMap->findFreeAddresses(usageMap->getOffset(), usageMap->getOffset() + usageMap->getSize() - 1, size)) {
      copy.range(block.from, block.to);
    }
    
    // No valid range left!
    if (!copy.ranges().size()) {
      return;
    }
  }

  addCleanedUpLocations(copy, result);
}

// ----------------------------------------------------------------------------
void PlacerMemoryMapBank::addCleanedUpLocations(const MemoryLocationConstraint &constraint, MemoryLocationList &result) {
  if (constraint.addresses().size()) {
    for (const auto &address : constraint.addresses()) {
      MemoryLocationConstraint item(constraint.isAllow());
      item.bank(index).address(address);
      result.add(item);
    }
    return;
  }

  if (constraint.ranges().size()) {
    for (const auto &range : constraint.ranges()) {
      uint64_t left = range.from;

      MemoryLocationConstraint leftItem(constraint.isAllow());
      leftItem.bank(index).address(left);
      result.add(leftItem);

      if (range.from != range.to) {
        MemoryLocationConstraint rightItem(constraint.isAllow());
        rightItem.bank(index).address(range.to);
        result.add(rightItem);
      }
/*
      left += 0x800;
      for (; left < range.to; left += 0x800) {
        MemoryLocationConstraint midItem(constraint.isAllow());
        midItem.bank(index).address(left);
        result.add(midItem);
      }
*/
    }

    return;
  }

  std::cout << "NO RANGE, NO ADDRESSES, JUST: " << constraint.asString() << std::endl;
}

// ----------------------------------------------------------------------------
void PlacerMemoryMapBank::dump(const std::string &prefix) {
  update();

  std::ostringstream os;
  os << prefix << "$" << std::hex << std::setw(2) << std::setfill('0') << (int)index << " : ";

  usageMap->dump(os.str());
}

// ----------------------------------------------------------------------------