#include <memory/MemoryPlacement.hpp>
#include <memory/MemoryMap.hpp>
#include <iostream>

using namespace FMA::memory;

// ----------------------------------------------------------------------------
MemoryPlacement::MemoryPlacement()
  : valid(false)
  , map(NULL)
{
}

// ----------------------------------------------------------------------------
MemoryPlacement::MemoryPlacement(MemoryMap *map)
  : valid(false)
  , map(map)
{
}

// ----------------------------------------------------------------------------
void MemoryPlacement::set(MemoryBankIndex _bank, const MemoryBankSize &_offset, const MemoryBankSize &_size) {
  bank = _bank;
  offset = _offset;
  size = _size;
  valid = true;
}

// ----------------------------------------------------------------------------
bool MemoryPlacement::isValid() const {
  return valid && map != NULL;
}

// ----------------------------------------------------------------------------
MemoryPlacement MemoryPlacement::withOffset(const int64_t &myOffset, const MemoryBankSize &size) const {
  MemoryPlacement copy(map);
  copy.set(bank, offset + myOffset, size);
  return copy;
}

// ----------------------------------------------------------------------------
uint64_t MemoryPlacement::asTranslatedAddress() const {
  if (map == NULL) {
    std::cerr << "Fatal error: Missing 'map' in memory placement" << std::endl;
    return 0;
  }

  return map->translateAddress(bank, offset);
}

// ----------------------------------------------------------------------------
