#ifndef __FMA_MEMORY_MEMORYMAPBANK_H__
#define __FMA_MEMORY_MEMORYMAPBANK_H__

#include <list>
#include "Configuration.hpp"
#include "MemoryMapShadow.hpp"

namespace FMA {
namespace memory {

class MemoryMapBankSection;
typedef std::list<MemoryMapBankSection*> MemoryMapBankSectionList;

class MemoryMapBank {  
public:
  MemoryMapBank(class MemoryMap *map, MemoryBankIndex bank, MemoryBankSize offset, MemoryBankSize size);
  virtual ~MemoryMapBank();

  MemoryMapBankSection *createSection(MemoryBankSize offset, MemoryBankSize size);
  void removeAllSections();
  inline void addShadow(MemoryMapShadow *shadow) { shadows.push_back(shadow); }
  bool hasAnyShadow() const;
  bool isPureShadowBank() const;

  inline const MemoryMapBankSectionList &getSections() const { return sections; }
  MemoryMapBankSection *getSectionAt(const MemoryBankSize &offset) const;
  MemoryMapShadowList getShadowsAt(const MemoryBankSize &offset) const;

  inline MemoryBankIndex getIndex() const { return index; }
  inline MemoryBankSize getOffset() const { return offset; }
  inline MemoryBankSize getSize() const { return size; }

  uint64_t translateAddress(MemoryBankSize address);

  void dump();

protected:
  class MemoryMap *map;

  MemoryBankIndex index;
  MemoryBankSize offset;
  MemoryBankSize size;

  MemoryMapBankSectionList sections;
  MemoryMapShadowList shadows;
};

}
}

#endif
