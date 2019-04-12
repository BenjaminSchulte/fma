#include <placer/Placer.hpp>
#include <placer/PlacerState.hpp>
#include <placer/PlacerRoute.hpp>
#include <placer/PlacerScopeNode.hpp>
#include <placer/PlacerStatePointer.hpp>
#include <memory/MemoryMap.hpp>
#include <fma/Project.hpp>
#include <iostream>

using namespace FMA;
using namespace FMA::placer;
using namespace FMA::memory;

// ----------------------------------------------------------------------------
Placer::Placer(Project *project, MemoryMap *map)
  : project(project)
  , map(map)
{
}

// ----------------------------------------------------------------------------
Placer::~Placer() {
}

// ----------------------------------------------------------------------------
void Placer::collectAllNodes(const PlacerNodePtr &scope, const PlacerStatePtr &state) {
  state->addNode(scope);

  for (auto &child : scope->getChildren()) {
    collectAllNodes(child, state);
  }
}

// ----------------------------------------------------------------------------
void Placer::collectAllNodes(const MemoryScopePtr &scope, const PlacerStatePtr &state) {
  PlacerNodePtr node(new PlacerScopeNode(this, state, scope));
  collectAllNodes(node, state);
}

// ----------------------------------------------------------------------------
void Placer::collectAllNodes(const PlacerStatePtr &state) {
  for (auto &node : map->scopes()) {
    collectAllNodes(node, state);
  }
}

// ----------------------------------------------------------------------------
bool Placer::placeRoute(const PlacerStatePtr &ptr, PlacerRoute &route) {
  route.selectState(ptr);
  route.rewind();

  PlacerNodeList::const_iterator it;
# ifdef DEBUG_FMA
    uint32_t nodeIndex = 0;
# endif
  for (it=ptr->getNodes().begin(); it!=ptr->getNodes().end(); it++) {
    if (!route.validNext()) {
      return false;
    }
    if (!it->get()->requiresPlacement()) {
      continue;
    }

    uint32_t variantIndex = route.next();

#   ifdef DEBUG_FMA
      project->log().trace() << "Placer: Node " << nodeIndex++ << " variant " << variantIndex;
#   endif

    if (!it->get()->place(variantIndex)) {
#     ifdef DEBUG_FMA
        project->log().trace() << "Placer: Failed node variant " << variantIndex;
#     endif
      continue;
    }
  }

  return true;
}

// ----------------------------------------------------------------------------
bool Placer::place() {
  PlacerRoute route;
  int32_t routeCounter = 0;
  bool succeeded = false;

  while (route.more() && routeCounter < PLACER_MAX_ITERATIONS) {
    project->log().debug() << "Placer: Current route index is " << ++routeCounter;

    PlacerState state(this);
    PlacerStatePtr ptr(&state);

    collectAllNodes(ptr);
    ptr->sort();

    if (placeRoute(ptr, route)) {
      succeeded = true;

      project->log().info() << "Successfully placed all nodes using " << routeCounter << " iteration(s)";
      // ptr->dumpNodes();
  
      break;
    }
    
    project->log().debug() << "Placer: Route " << routeCounter << " failed. Trying next route.";

    if (routeCounter == PLACER_MAX_ITERATIONS) {
      project->log().error() << "Could not place all items after testing " << PLACER_MAX_ITERATIONS << " variants.";
      ptr->dumpNodes();
      ptr->dumpMemoryUsage();
      map->dump();
    }
  }

  return succeeded;
}

// ----------------------------------------------------------------------------
