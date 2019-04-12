#ifndef __FMA_MEMORY_MEMORYMAP_H__
#define __FMA_MEMORY_MEMORYMAP_H__

#include <map>
#include "MemoryAllocationList.hpp"
#include "MemoryScope.hpp"
#include "MemoryMapBank.hpp"
#include "MemoryMapShadow.hpp"

namespace FMA {
namespace memory {

typedef std::map<MemoryBankIndex, MemoryMapBank*> MemoryMapBankList;

class MemoryMap {  
public:
  MemoryMap(class MemoryAdapter *adapter);
  virtual ~MemoryMap();

  const MemoryScopeList &scopes() { return _scopes; }
  const MemoryMapBankList &getBanks() { return banks; }
  MemoryMapBank *createBank(MemoryBankIndex bank, MemoryBankSize offset, MemoryBankSize size);
  MemoryMapBank *getBank(MemoryBankIndex bank);
  void removeAllBanks();
  void removeAllShadows();

  MemoryMapShadow *createShadow(MemoryBankIndex left, MemoryBankSize leftAddress, MemoryBankIndex right, MemoryBankSize rightAddress, MemoryBankSize size);
  MemoryScopePtr createMemoryScope();
  MemoryScopePtr createLooseMemoryScope();

  bool place();

  void dump();
  void dumpScopes();
  void useForDynamicMemory();
  void useForStaticMemory();

  uint64_t translateAddress(MemoryBankIndex bank, MemoryBankSize offset);

  inline class MemoryAdapter *getAdapter() const { return adapter; }

protected:
  MemoryScopeList _scopes;
  class MemoryAdapter *adapter;

  MemoryMapBankList banks;
  MemoryMapShadowList shadows;

  bool isDynamic;
  bool isStatic;
};

typedef std::vector<MemoryMap*> MemoryMapList;

}
}

#endif
