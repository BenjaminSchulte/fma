#ifndef __FMA_MEMORY_MEMORYLOCATIONLIST_H__
#define __FMA_MEMORY_MEMORYLOCATIONLIST_H__

#include <vector>
#include <fma/plugin/MemoryPluginAdapter.hpp>
#include "MemoryLocationConstraint.hpp"

namespace FMA {
namespace memory {

typedef std::vector<MemoryLocationConstraint> MemoryLocationConstraints;

class MemoryLocationList : public plugin::MemoryLocation {  
public:
  virtual void overrideAllow(const interpret::ContextPtr &, const interpret::GroupedParameterList &);
  virtual void allow(const interpret::ContextPtr &, const interpret::GroupedParameterList &);
  virtual void deny(const interpret::ContextPtr &, const interpret::GroupedParameterList &);

  inline const MemoryLocationConstraints &getAllowed() const { return allowed; }
  inline const MemoryLocationConstraints &getDenied() const { return denied; }

  void add(const MemoryLocationConstraint &item);

  bool isStaticAddressLocation() const;
  bool isStaticBankLocation() const;

  MemoryLocationList applyMemoryShadows(class MemoryMap *) const;
  MemoryLocationList merge(const MemoryLocationList &other) const;
  MemoryLocationConstraints merge(const MemoryLocationConstraints &left, const MemoryLocationConstraints &right) const;

  void dump() const;

protected:
  MemoryLocationConstraints allowed;
  MemoryLocationConstraints denied;
};

}
}

#endif
