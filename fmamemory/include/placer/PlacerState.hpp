#ifndef __FMA_PLACER_PLACERSTATE_H__
#define __FMA_PLACER_PLACERSTATE_H__

#include "PlacerNode.hpp"

namespace FMA {
namespace memory {
class MemoryAllocation;
typedef std::shared_ptr<MemoryAllocation> MemoryAllocationPtr;
}
namespace placer {

class PlacerMemoryMap;

class PlacerStatePtr;
class PlacerState {
public:
  PlacerState(class Placer*);
  virtual ~PlacerState();

  void addNode(const PlacerNodePtr &node);
  void sort();
  void dumpNodes();
  void dumpMemoryUsage();

  uint32_t getNumNodes() const { return nodes.size(); }
  const PlacerNodeList &getNodes() const { return nodes; }

  inline const PlacerMemoryMapPtr &getMemoryMap() const { return memoryMap; }

  PlacerNodePtr createPlacerNode(const PlacerStatePtr &state, const FMA::memory::MemoryAllocationPtr &child);

protected:
  Placer *placer;
  PlacerNodeList nodes;
  PlacerMemoryMapPtr memoryMap;
  std::map<const memory::MemoryAllocation*, PlacerNodePtr> nodePointerMap;

  void removeAllNodes();
};

}
}

#endif
