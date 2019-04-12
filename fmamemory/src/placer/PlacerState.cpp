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
