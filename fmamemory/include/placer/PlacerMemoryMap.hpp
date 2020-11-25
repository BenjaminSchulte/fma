#ifndef __FMA_PLACER_PLACERMEMORYMAP_H__
#define __FMA_PLACER_PLACERMEMORYMAP_H__

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <memory/MemoryLocationList.hpp>

namespace FMA {
namespace placer {

class Placer;
class PlacerMemoryMap;
class PlacerMemoryMapBank;
class UsageMap;
class MergedUsageMap;

typedef std::shared_ptr<PlacerMemoryMap> PlacerMemoryMapPtr;
typedef std::weak_ptr<PlacerMemoryMap> WeakPlacerMemoryMapPtr;
typedef std::vector<PlacerMemoryMapPtr> PlacerMemoryMapList;
typedef std::vector<WeakPlacerMemoryMapPtr> WeakPlacerMemoryMapList;

typedef std::map<memory::MemoryBankIndex, PlacerMemoryMapBank*> PlacerMemoryMapBankList;
typedef std::map<memory::MemoryBankIndex, UsageMap*> UsageMapList;
typedef std::map<memory::MemoryBankIndex, MergedUsageMap*> MergedUsageMapList;

class PlacerMemoryMap : public std::enable_shared_from_this<PlacerMemoryMap> {
public:
  PlacerMemoryMap(Placer *placer, bool shared) : placer(placer), mShared(shared) {}
  virtual ~PlacerMemoryMap();

  void addChild(const PlacerMemoryMapPtr &child);
  void addParentWithoutChild(const WeakPlacerMemoryMapPtr &parent);

  virtual void dump() const;
  virtual void dump(const std::string &prefix) const;

  virtual bool place(const memory::MemoryLocationConstraint &location, const memory::MemoryBankSize &size);
  virtual bool place(const memory::MemoryBankIndex bank, const memory::MemoryBankSize &offset, const memory::MemoryBankSize &size);

  virtual void filterValidLocations(const memory::MemoryLocationConstraint &constraint, memory::MemoryLocationList &result, const memory::MemoryBankSize &size);
  virtual memory::MemoryLocationList filterValidLocations(const memory::MemoryLocationList &locations, const memory::MemoryBankSize &size);

//  virtual void updateUsageMap(memory::MemoryBankIndex bank, UsageMap *usageMap);
  virtual MergedUsageMap *getUsageMap(memory::MemoryBankIndex bank);

  virtual PlacerMemoryMap *closestMemoryMapOwnerOrNull(bool includeSelf);
  virtual PlacerMemoryMap *closestMemoryMapOwner();
  virtual bool needsOwnUsageMap() const;

  bool isShared() const;
  bool isParentShared() const;

  inline const std::string &getNameHint() const { return name; }
  inline void setNameHint(const std::string &_name) { name=_name; }
  inline const WeakPlacerMemoryMapList &getParents() const { return parents; }

protected:
  Placer *placer;
  bool mShared;
  PlacerMemoryMapList children;
  WeakPlacerMemoryMapList parents;
  
  PlacerMemoryMapBankList ownBanks;
  UsageMapList usageMaps;
  MergedUsageMapList mergedUsageMaps;

  std::string name;

  virtual void removeAllBanks();

  /// Returns the relevant memory map used to reserve content inside this scope
  virtual PlacerMemoryMapBank *getMemoryMap(memory::MemoryBankIndex bank);

  virtual PlacerMemoryMapBank *createOwnMemoryMap(memory::MemoryBankIndex bank);
  virtual bool placeWithoutShadowTest(const memory::MemoryBankIndex bank, const memory::MemoryBankSize &offset, const memory::MemoryBankSize &size);
  virtual UsageMap *createUsageMap(memory::MemoryBankIndex bank);
};

}
}

#endif
