#ifndef __FMA_PLACER_PLACERSTATE_H__
#define __FMA_PLACER_PLACERSTATE_H__

#include "PlacerNode.hpp"

namespace FMA {
namespace placer {

class PlacerMemoryMap;

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

protected:
  Placer *placer;
  PlacerNodeList nodes;
  PlacerMemoryMapPtr memoryMap;

  void removeAllNodes();
};

}
}

#endif
