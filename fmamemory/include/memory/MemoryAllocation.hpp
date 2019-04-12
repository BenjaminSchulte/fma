#ifndef __FMA_MEMORY_MEMORYALLOCATION_H__
#define __FMA_MEMORY_MEMORYALLOCATION_H__

#include <memory>
#include <fma/symbol/SymbolReference.hpp>
#include "./MemoryLocationList.hpp"
#include "MemoryPlacement.hpp"

namespace FMA {
namespace memory {

class MemoryMap;

typedef std::weak_ptr<class MemoryAllocation> WeakMemoryAllocationPtr;
typedef std::shared_ptr<class MemoryAllocation> MemoryAllocationPtr;
typedef std::shared_ptr<class MemoryScope> MemoryScopePtr;

class MemoryAllocation : public std::enable_shared_from_this<MemoryAllocation> {  
public:
  MemoryAllocation(MemoryMap *map);
  virtual ~MemoryAllocation();

  virtual MemoryScopePtr asMemoryScope() { return MemoryScopePtr(); }

  virtual void writeSymbolReferences(class MemorySymbolMap *symbols);

  inline void setNameHint(const std::string &_name) { name=_name; }
  inline void setReference(const symbol::SymbolReferencePtr &_reference) { reference=_reference; }
  inline bool hasReference() const { return !!reference; }
  inline const symbol::SymbolReferencePtr &getReference() const { return reference; }
  inline const std::string &getNameHint() const { return name; }

  MemoryLocationList &locations() { return _locations; }
  inline void copyLocationFrom(const MemoryLocationList &other) { _locations = other; }
  void addParent(const WeakMemoryAllocationPtr &);

  virtual void dump(const std::string &prefix);
  virtual void dumpRow(const std::string &prefix);
  virtual void dumpRowNoLocation(const std::string &prefix);

  void setSize(const MemoryBankSize &size);
  void setClass(const types::ClassPtr &klass);
  inline bool isClass() const { return !!klass; }
  const types::ClassPtr &getClass() const {return klass;}

  inline bool isArray() const { return hasArraySize; }
  void setArraySize(const MemoryBankSize &size);
  inline const MemoryBankSize &getArraySize() const { return arraySize; }
  virtual bool requiresPlacement() { return hasValidSize && getSize() > 0; }
  virtual MemoryBankSize getItemSize();
  virtual MemoryBankSize getSize();

  inline std::shared_ptr<MemoryAllocation> getPointer() {
    return shared_from_this();
  }

  inline MemoryPlacement *placement() const { return _placement; };

protected:
  MemoryLocationList _locations;
  std::vector<WeakMemoryAllocationPtr> parentScopes;
  symbol::SymbolReferencePtr reference;

  MemoryMap *map;

  std::string name;
  MemoryPlacement *_placement;

  bool hasValidSize;
  MemoryBankSize size;

  bool hasArraySize;
  MemoryBankSize arraySize;

  bool hasCachedSize;
  MemoryBankSize cachedSize;
  types::ClassPtr klass;

  bool hasSortedMembers;
};

}
}

#endif
