#ifndef __FMA_MEMORY_MEMORYMAPBANKSECTION_H__
#define __FMA_MEMORY_MEMORYMAPBANKSECTION_H__

#include "Configuration.hpp"
#include "MemoryMapShadow.hpp"

namespace FMA {
namespace memory {

class MemoryMapBankSection {
public:
  MemoryMapBankSection(MemoryBankSize offset, MemoryBankSize size);
  virtual ~MemoryMapBankSection();

  inline bool hasLocation() const { return useLocation; }
  inline uint64_t getLocation() const { return location; }
  inline void setLocatedAt(uint64_t _location) {
    useLocation = true;
    location = _location;
  }

  inline void setIsShadow(MemoryMapShadow *_shadow) { shadow=_shadow;}
  inline bool isShadow() const { return !!shadow; }
  inline MemoryMapShadow *getShadow() const { return shadow; }

  inline const MemoryBankSize &getOffset() const { return offset; }
  inline const MemoryBankSize &getSize() const { return size; }

  void dump(MemoryMapBank *bank, bool first);

protected:
  MemoryBankSize offset;
  MemoryBankSize size;

  MemoryMapShadow *shadow;
  bool useLocation;
  uint64_t location;
};

}
}

#endif
