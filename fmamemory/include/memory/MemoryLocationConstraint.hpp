#ifndef __FMA_MEMORY_MEMORYLOCATIONCONSTRAINT_H__
#define __FMA_MEMORY_MEMORYLOCATIONCONSTRAINT_H__

#include "Configuration.hpp"
#include <fma/plugin/MemoryPluginAdapter.hpp>

namespace FMA {
namespace memory {

struct MemoryLocationRange {
  MemoryLocationRange() : from(0), to(0) {}
  MemoryLocationRange(const MemoryBankSize &from, const MemoryBankSize &to) : from(from), to(to) {}

  MemoryBankSize from;
  MemoryBankSize to;
};

class MemoryLocationConstraint {
public:
  MemoryLocationConstraint();
  MemoryLocationConstraint(bool isAllow);
  MemoryLocationConstraint(const interpret::ContextPtr &, const interpret::GroupedParameterList &, bool isAllow);
  virtual ~MemoryLocationConstraint();

  void configure(const interpret::ContextPtr &, const interpret::GroupedParameterList &);
  bool isEmptyConstraint() const;
  std::string asString() const;

  MemoryLocationConstraint &anyAlignment();
  MemoryLocationConstraint &alignedTo(uint32_t alignment, uint32_t offset=0);
  inline bool hasAlignment() const { return !!_alignment; }
  bool alignmentMatches(uint64_t address) const;

  MemoryLocationConstraint &anyBank();
  MemoryLocationConstraint &bank(MemoryBankIndex bank);
  MemoryLocationConstraint &banks(MemoryBankIndex from, MemoryBankIndex to);
  inline const std::vector<MemoryBankIndex> &banks() const { return _banks; }

  MemoryLocationConstraint &anyAddress();
  MemoryLocationConstraint &anyRange();
  MemoryLocationConstraint &address(const MemoryBankSize &address);
  MemoryLocationConstraint &range(const MemoryBankSize &from, const MemoryBankSize &to);
  inline const std::vector<MemoryBankSize> &addresses() const { return _addresses; }
  inline const std::vector<MemoryLocationRange> &ranges() const { return _ranges; }

  inline const MemoryBankSize &alignment() const { return _alignment; }
  inline const MemoryBankSize &alignmentOffset() const { return _alignmentOffset; }

  void overrideBy(const MemoryLocationConstraint &other);

  void merge(const MemoryLocationConstraint &other, std::vector<MemoryLocationConstraint> &result) const;
  void mergeBanks(const MemoryLocationConstraint &other, std::vector<MemoryLocationConstraint> &result) const;
  void mergeAddresses(const MemoryLocationConstraint &other, std::vector<MemoryLocationConstraint> &result) const;
  void mergeRanges(const MemoryLocationConstraint &other, std::vector<MemoryLocationConstraint> &result) const;
  void mergeAlign(const MemoryLocationConstraint &other, std::vector<MemoryLocationConstraint> &result) const;
  void mergeRemoveAddressesForRange(const MemoryLocationConstraint &other, std::vector<MemoryLocationConstraint> &result) const;

  inline bool isAllow() const { return _isAllow; }
  bool isStaticBank() const;
  bool isStaticAddress() const;

  void applyMemoryShadows(class MemoryMap *, class MemoryLocationList &) const;
  void applySingleBankMemoryShadow(MemoryBankIndex index, class MemoryMap *map, class MemoryLocationList &newList);

  bool serialize(FMA::output::DynamicBuffer &) const;
  bool deserialize(Log *log, FMA::output::DynamicBuffer &);

protected:
  std::vector<MemoryBankIndex> _banks;
  std::vector<MemoryBankSize> _addresses;
  std::vector<MemoryLocationRange> _ranges;
  
  MemoryBankSize _alignment;
  MemoryBankSize _alignmentOffset;

  bool _isAllow;
};

}
}

#endif
