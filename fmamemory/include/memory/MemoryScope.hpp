#ifndef __FMA_MEMORY_MEMORYSCOPE_H__
#define __FMA_MEMORY_MEMORYSCOPE_H__

#include "MemoryAllocationList.hpp"

namespace FMA {
namespace memory {

typedef std::shared_ptr<class MemoryScope> MemoryScopePtr;
typedef std::vector<MemoryScopePtr> MemoryScopeList;

class MemoryScope : public MemoryAllocation {  
public:
  MemoryScope(MemoryMap *map, bool shared);
  virtual ~MemoryScope();

  virtual MemoryScopePtr asMemoryScope() { return getScopePointer(); }

  MemoryAllocationList &children() { return _children; }

  virtual void writeSymbolReferences(class MemorySymbolMap *symbols);

  MemoryScopePtr createScope(bool shared);
  void addChild(const MemoryAllocationPtr &);
  virtual void dump(const std::string &prefix);

  inline std::shared_ptr<MemoryScope> getScopePointer() {
    return std::dynamic_pointer_cast<MemoryScope>(getPointer());
  }

  inline bool isShared() const { return mShared; }

protected:
  MemoryAllocationList _children;
  bool mShared;
};

}
}

#endif
