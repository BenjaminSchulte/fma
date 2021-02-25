#include <placer/Placer.hpp>
#include <placer/PlacerNode.hpp>
#include <placer/PlacerScopeNode.hpp>
#include <placer/PlacerState.hpp>
#include <placer/PlacerPriority.hpp>
#include <placer/RootPlacerMemoryMap.hpp>
#include <iostream>

using namespace FMA::placer;

// ----------------------------------------------------------------------------
PlacerState::PlacerState(Placer *placer)
  : placer(placer)
  , memoryMap(new RootPlacerMemoryMap(placer))
{
}

// ----------------------------------------------------------------------------
PlacerState::~PlacerState() {
  removeAllNodes();
}

// ----------------------------------------------------------------------------
void PlacerState::addNode(const PlacerNodePtr &node) {
  nodes.push_back(node);
}

// ----------------------------------------------------------------------------
void PlacerState::removeAllNodes() {
  nodes.clear();
}

// ----------------------------------------------------------------------------
void PlacerState::sort() {
  nodes.sort(__sortNodes);
}

// ----------------------------------------------------------------------------
void PlacerState::dumpNodes() {
  for (const auto &node : nodes) {
    node->dump();
  }
}

// ----------------------------------------------------------------------------
void PlacerState::dumpMemoryUsage() {
  memoryMap->dump();
}

// ----------------------------------------------------------------------------
PlacerNodePtr PlacerState::createPlacerNode(const PlacerStatePtr &state, const FMA::memory::MemoryAllocationPtr &child) {
  const memory::MemoryAllocation *key = child.get();
  std::map<const memory::MemoryAllocation*, PlacerNodePtr>::iterator it = nodePointerMap.find(key);
  if (it != nodePointerMap.end()) {
    return it->second;
  }

  const auto &ptr = child->asMemoryScope();
  PlacerNodePtr node;
  if (ptr) {
    node = PlacerNodePtr(new PlacerScopeNode(placer, state, ptr));
  } else {
    node = PlacerNodePtr(new PlacerNode(placer, state, child));
  }

  nodePointerMap[key] = node;
  return node;
}

// ----------------------------------------------------------------------------
