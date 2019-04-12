#include <memory/MemoryMap.hpp>
#include <memory/MemoryAdapter.hpp>
#include <memory/MemoryMapBank.hpp>
#include <memory/MemoryMapBankSection.hpp>
#include <fma/Project.hpp>
#include <iostream>

using namespace FMA::memory;

// ----------------------------------------------------------------------------
MemoryMapBank::MemoryMapBank(MemoryMap *map, MemoryBankIndex index, MemoryBankSize offset, MemoryBankSize size)
  : map(map)
  , index(index)
  , offset(offset)
  , size(size)
{
}

// ----------------------------------------------------------------------------
MemoryMapBank::~MemoryMapBank() {
  removeAllSections();
}

// ----------------------------------------------------------------------------
void MemoryMapBank::removeAllSections() {
  for (auto &item : sections) {
    MemoryMapBankSection *sect = item;
    delete sect;
  }

  sections.clear();
}

// ----------------------------------------------------------------------------
MemoryMapBankSection *MemoryMapBank::createSection(MemoryBankSize offset, MemoryBankSize size) {
  MemoryMapBankSection *section = new MemoryMapBankSection(offset, size);
  sections.push_back(section);
  return section;
}

// ----------------------------------------------------------------------------
MemoryMapShadowList MemoryMapBank::getShadowsAt(const MemoryBankSize &offset) const {
  MemoryMapShadowList result;

  for (auto &shadow : shadows) {
    const MemoryBankSize &address = shadow->getOwnAddress(this);

    if (address <= offset && (address + shadow->getSize() -1) >= offset) {
      result.push_back(shadow);
    }
  }

  return result;
}

// ----------------------------------------------------------------------------
MemoryMapBankSection *MemoryMapBank::getSectionAt(const MemoryBankSize &offset) const {
  for (const auto &section : sections) {
    if (section->getOffset() <= offset && (section->getOffset() + section->getSize() - 1) >= offset) {
      return section;
    }
  }

  return NULL;
}

// ----------------------------------------------------------------------------
bool MemoryMapBank::hasAnyShadow() const {
  for (auto &section : sections) {
    if (section->isShadow()) {
      return true;
    }
  }
  return false;
}

// ----------------------------------------------------------------------------
uint64_t MemoryMapBank::translateAddress(MemoryBankSize address) {
  MemoryMapBank *current = this;

  while (true) {
    MemoryMapBankSection *section = current->getSectionAt(address);
    if (!section) {
      map->getAdapter()->getProject()->log().error() << "Unable to find section of address " << address << " in bank " << (uint64_t)current->getIndex();
      return 0;
    }

    if (!section->hasLocation() && section->isShadow()) {
      address = (int64_t)address - (int64_t)section->getShadow()->getOwnAddress(current) + (int64_t)section->getShadow()->getOtherAddress(current);
      current = section->getShadow()->getOtherBank(current);
      continue;
    }

    if (!section->hasLocation()) {
      map->getAdapter()->getProject()->log().error() << "Could not translate address";
      return 0;
    }

    return section->getLocation() + address - section->getOffset();
  }
}

// ----------------------------------------------------------------------------
bool MemoryMapBank::isPureShadowBank() const {
  for (auto &section : sections) {
    if (!section->isShadow()) {
      return false;
    }
  }
  return true;
}

// ----------------------------------------------------------------------------
void MemoryMapBank::dump() {
  bool first = true;
  for (auto &section : sections) {
    section->dump(this, first);
    first = false;
  }
}

// ----------------------------------------------------------------------------
