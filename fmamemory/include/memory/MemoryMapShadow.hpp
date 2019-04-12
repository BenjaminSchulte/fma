#ifndef __FMA_MEMORY_MEMORYMAPSHADOW_H__
#define __FMA_MEMORY_MEMORYMAPSHADOW_H__

#include <vector>
#include "Configuration.hpp"

namespace FMA {
namespace memory {

class MemoryMapBank;

class MemoryMapShadow {  
public:
  MemoryMapShadow(MemoryMapBank *left, MemoryMapBank *right, MemoryBankSize leftAddress, MemoryBankSize rightAddress, MemoryBankSize size);
  virtual ~MemoryMapShadow();

  inline MemoryMapBank *getLeftBank() const { return left; }
  inline MemoryMapBank *getRightBank() const { return right; }
  inline MemoryBankSize getLeftAddress() const { return leftAddress; }
  inline MemoryBankSize getRightAddress() const { return rightAddress; }
  inline MemoryBankSize getSize() const { return size; };
  
  MemoryMapBank *getOtherBank(const MemoryMapBank *bank);
  MemoryBankSize getOwnAddress(const MemoryMapBank *bank);
  MemoryBankSize getOtherAddress(const MemoryMapBank *bank);

protected:
  MemoryMapBank *left;
  MemoryMapBank *right;
  MemoryBankSize leftAddress;
  MemoryBankSize rightAddress;
  MemoryBankSize size;
};

typedef std::vector<MemoryMapShadow*> MemoryMapShadowList;

}
}

#endif
