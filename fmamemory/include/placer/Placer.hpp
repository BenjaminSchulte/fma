#ifndef __FMA_PLACER_H__
#define __FMA_PLACER_H__

#define PLACER_MAX_ITERATIONS 64

#include <memory>

namespace FMA {
class Project;
namespace memory {
class MemoryMap;
class MemoryScope;
typedef std::shared_ptr<MemoryScope> MemoryScopePtr;
}
namespace placer {
class PlacerStatePtr;
class PlacerRoute;
class PlacerNode;
typedef std::shared_ptr<PlacerNode> PlacerNodePtr;

class Placer {
public:
  Placer(Project *project, memory::MemoryMap *map);
  virtual ~Placer();

  bool placeRoute(const PlacerStatePtr &, PlacerRoute &);
  bool place();

  inline Project *getProject() const { return project; }
  inline memory::MemoryMap *getMemoryMap() const { return map; };

protected:
  Project *project;
  memory::MemoryMap *map;

  void collectAllNodes(const PlacerNodePtr &scope, const PlacerStatePtr &state);
  void collectAllNodes(const memory::MemoryScopePtr &scope, const PlacerStatePtr &state);
  void collectAllNodes(const PlacerStatePtr &state);

  void scoreAllNodes(const PlacerStatePtr &state);
};

}
}

#endif
