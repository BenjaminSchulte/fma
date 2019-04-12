#include <placer/PlacerScopeNode.hpp>
#include <placer/PlacerState.hpp>
#include <placer/PlacerStatePointer.hpp>
#include <memory/MemoryScope.hpp>
#include <iostream>

using namespace FMA::placer;
using namespace FMA::memory;

// ----------------------------------------------------------------------------
PlacerScopeNode::PlacerScopeNode(Placer *placer, const PlacerStatePtr &state, const memory::MemoryScopePtr &scope)
  : PlacerNode(placer, state, scope)
  , scope(scope)
  , didCollectChildren(false)
{
}

// ----------------------------------------------------------------------------
PlacerScopeNode::~PlacerScopeNode() {

}

// ----------------------------------------------------------------------------
const PlacerNodeList &PlacerScopeNode::getChildren() {
  if (!didCollectChildren) {
    didCollectChildren = true;

    for (const auto &child : scope->children()) {
      const MemoryScopePtr &ptr = child->asMemoryScope();
      PlacerNodePtr node;
      if (ptr) {
        node = PlacerNodePtr(new PlacerScopeNode(placer, state, ptr));
      } else {
        node = PlacerNodePtr(new PlacerNode(placer, state, child));
      }

      node->addParent(std::dynamic_pointer_cast<PlacerScopeNode>(shared_from_this()));
      children.push_back(node);
    }
  }

  return children;
}

// ----------------------------------------------------------------------------
const PlacerMemoryMapPtr &PlacerScopeNode::getMemoryMap() {
  if (memoryMap) {
    return memoryMap;
  }

  memoryMap = PlacerMemoryMapPtr(new PlacerMemoryMap(placer));
  memoryMap->setNameHint(node->getNameHint());
  if (!parents.size()) {
    state->getMemoryMap()->addChild(memoryMap);
  } else {
    for (const auto &parent : parents) {
      const auto &item = parent.lock();
      if (item) {
        item->getMemoryMap()->addChild(memoryMap);
      }
    }
  }

  return memoryMap;
}

// ----------------------------------------------------------------------------
