#ifndef __FMA_PLACER_PLACERNODE_H__
#define __FMA_PLACER_PLACERNODE_H__

#include <list>
#include <memory>
#include <memory/MemoryAllocation.hpp>
#include <memory/MemoryLocationList.hpp>

namespace FMA {
namespace placer {

class Placer;
class PlacerStatePtr;

typedef std::shared_ptr<class PlacerNode> PlacerNodePtr;
typedef std::shared_ptr<class PlacerScopeNode> PlacerScopeNodePtr;
typedef std::shared_ptr<class PlacerMemoryMap> PlacerMemoryMapPtr;
typedef std::weak_ptr<class PlacerScopeNode> WeakPlacerScopeNodePtr;
typedef std::list<PlacerNodePtr> PlacerNodeList;
typedef std::vector<WeakPlacerScopeNodePtr> PlacerNodeParentList;

class PlacerNode : public std::enable_shared_from_this<PlacerNode> {
public:
  PlacerNode(Placer *placer, const PlacerStatePtr &state, const memory::MemoryAllocationPtr &node);
  virtual ~PlacerNode();

  virtual const PlacerNodeList &getChildren() { return children; }
  virtual const PlacerNodeParentList &getParents() { return parents; }
  virtual void addParent(const PlacerScopeNodePtr &parent) { parents.push_back(parent); }
  virtual void dump();

  virtual const memory::MemoryAllocationPtr &getNode() const { return node; }

  bool hasStaticAddress();
  bool hasStaticBank();

  virtual bool requiresPlacement() const;
  virtual bool isMemberOf(const PlacerNodePtr &other);

  const memory::MemoryLocationList &getLocation();
  const memory::MemoryLocationList &getShadowedLocation();
  
  bool collectValidPlacements();
  uint32_t getNumValidPlacements();
  bool place(uint32_t placement);

  virtual int64_t score();

  virtual const PlacerMemoryMapPtr &getMemoryMap();

protected:
  memory::MemoryAllocationPtr node;
  PlacerNodeList children;
  PlacerNodeParentList parents;

  bool hasCachedLocation;
  memory::MemoryLocationList location;

  bool hasCachedShadowedLocation;
  memory::MemoryLocationList shadowedLocation;

  bool hasCachedValidLocations;
  memory::MemoryLocationList validLocations;

  Placer *placer;
  const PlacerStatePtr &state;

  bool hasCachedScore;
  int64_t _score;

  virtual int64_t calculateScore();
};

}
}

#endif
