#ifndef __FMA_MEMORY_MEMORYPLACEMENT_H__
#define __FMA_MEMORY_MEMORYPLACEMENT_H__

#include "Configuration.hpp"
#include <fma/plugin/MemoryPluginAdapter.hpp>

namespace FMA {
namespace memory {

class MemoryMap;

class MemoryPlacement : public FMA::plugin::MemoryBlockPlacement {
public:
  MemoryPlacement();
  MemoryPlacement(MemoryMap *map);

  void set(MemoryBankIndex bank, const MemoryBankSize &offset, const MemoryBankSize &size);

  inline MemoryBankIndex getBank() const { return bank; }
  inline const MemoryBankSize &getOffset() const { return offset; }

  MemoryPlacement withOffset(const int64_t &offset, const MemoryBankSize &size) const;

  uint64_t asLongAddress() const { return offset + (bank << 16); }
  uint64_t asTranslatedAddress() const;

  bool isValid() const;

protected:
  bool valid;
  MemoryMap *map;
  MemoryBankIndex bank;
  MemoryBankSize offset;
  MemoryBankSize size;
};

}
}

#endif