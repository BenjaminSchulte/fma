#ifndef __FMA_PLACER_PLACERSCOPENODE_H__
#define __FMA_PLACER_PLACERSCOPENODE_H__

#include "PlacerNode.hpp"
#include "PlacerMemoryMap.hpp"
#include <memory/MemoryScope.hpp>

namespace FMA {
namespace placer {

class PlacerScopeNode : public PlacerNode {
public:
  PlacerScopeNode(Placer *placer, const PlacerStatePtr &state, const memory::MemoryScopePtr &scope);
  virtual ~PlacerScopeNode();

  const PlacerNodeList &getChildren();
  bool containsChildRecursive(const PlacerNodePtr &other);

  const PlacerMemoryMapPtr &getMemoryMap();

protected:
  memory::MemoryScopePtr scope;
  bool didCollectChildren;

  PlacerMemoryMapPtr memoryMap;
};

}
}

#endif
