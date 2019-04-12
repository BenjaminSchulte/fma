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

typedef std::shared_ptr<PlacerMemoryMap> PlacerMemoryMapPtr;
typedef std::weak_ptr<PlacerMemoryMap> WeakPlacerMemoryMapPtr;
typedef std::vector<PlacerMemoryMapPtr> PlacerMemoryMapList;
typedef std::vector<WeakPlacerMemoryMapPtr> WeakPlacerMemoryMapList;

typedef std::map<memory::MemoryBankIndex, PlacerMemoryMapBank*> PlacerMemoryMapBankList;

class PlacerMemoryMap : public std::enable_shared_from_this<PlacerMemoryMap> {
public:
  PlacerMemoryMap(Placer *placer) : placer(placer) {}
  virtual ~PlacerMemoryMap();

  void addChild(const PlacerMemoryMapPtr &child);
  void addParentWithoutChild(const WeakPlacerMemoryMapPtr &parent);

  virtual void dump() const;
  virtual void dump(const std::string &prefix) const;

  /// Returns the relevant memory map used to reserve content inside this scope
  virtual PlacerMemoryMapBank *getReadMemoryMap(memory::MemoryBankIndex bank);

  /// Returns the relevant memory map used to reserve content inside this scope
  virtual PlacerMemoryMapBank *getWriteMemoryMap(memory::MemoryBankIndex bank);

  /// Returns the own memory map
  virtual PlacerMemoryMapBank *getOwnMemoryMap(memory::MemoryBankIndex bank);

  virtual bool place(const memory::MemoryLocationConstraint &location, const memory::MemoryBankSize &size);
  virtual bool place(const memory::MemoryBankIndex bank, const memory::MemoryBankSize &offset, const memory::MemoryBankSize &size);

  virtual void filterValidLocations(const memory::MemoryLocationConstraint &constraint, memory::MemoryLocationList &result, const memory::MemoryBankSize &size);
  virtual memory::MemoryLocationList filterValidLocations(const memory::MemoryLocationList &locations, const memory::MemoryBankSize &size);

  virtual UsageMap *createUsageMap(memory::MemoryBankIndex bank);
  virtual void updateUsageMap(memory::MemoryBankIndex bank, UsageMap *usageMap);

  inline void setNameHint(const std::string &_name) { name=_name; }

protected:
  Placer *placer;
  PlacerMemoryMapList children;
  WeakPlacerMemoryMapList parents;
  
  PlacerMemoryMapBankList ownBanks;

  std::string name;

  virtual void removeAllBanks();

  virtual PlacerMemoryMapBank *createOwnMemoryMap(memory::MemoryBankIndex bank);
  virtual bool placeWithoutShadowTest(const memory::MemoryBankIndex bank, const memory::MemoryBankSize &offset, const memory::MemoryBankSize &size);
};

}
}

#endif
