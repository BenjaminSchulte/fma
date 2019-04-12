#include <placer/PlacerNode.hpp>
#include <placer/PlacerScopeNode.hpp>
#include <placer/PlacerState.hpp>
#include <placer/PlacerStatePointer.hpp>
#include <placer/Placer.hpp>
#include <memory/MemoryAllocation.hpp>
#include <fma/Project.hpp>
#include <fma/util/term.hpp>
#include <iostream>
#include <iomanip>

using namespace FMA::placer;
using namespace FMA::memory;
using namespace FMA::util;

// ----------------------------------------------------------------------------
PlacerNode::PlacerNode(Placer *placer, const PlacerStatePtr &state, const MemoryAllocationPtr &node)
  : node(node)
  , hasCachedLocation(false)
  , hasCachedShadowedLocation(false)
  , hasCachedValidLocations(false)
  , placer(placer)
  , state(state)
  , hasCachedScore(false)
{
}

// ----------------------------------------------------------------------------
PlacerNode::~PlacerNode() {
}

// ----------------------------------------------------------------------------
void PlacerNode::dump() {
  std::cout << std::setw(5) << score() << " + ";
  node->dumpRowNoLocation("");

  for (const auto &allow : getLocation().getAllowed()) {
    std::cout << color::green() << " +" << allow.asString() << color::off();
  }
  for (const auto &deny : getLocation().getDenied()) {
    std::cout << color::red() << " -" << deny.asString() << color::off();
  }

  std::cout << std::endl;
}

// ----------------------------------------------------------------------------
int64_t PlacerNode::score() {
  if (!hasCachedScore) {
    _score = calculateScore();
  }

  return _score;
}

// ----------------------------------------------------------------------------
int64_t PlacerNode::calculateScore() {
  return node->requiresPlacement() ? node->getSize() : 0;
}

// ----------------------------------------------------------------------------
bool PlacerNode::requiresPlacement() const {
  return node->requiresPlacement();
}

// ----------------------------------------------------------------------------
bool PlacerNode::hasStaticAddress() {
  return getLocation().isStaticAddressLocation();
}

// ----------------------------------------------------------------------------
bool PlacerNode::hasStaticBank() {
  return getLocation().isStaticBankLocation();
}

// ----------------------------------------------------------------------------
const MemoryLocationList &PlacerNode::getLocation() {
  if (hasCachedLocation) {
    return location;
  }

  location = node->locations();
  if (location.getAllowed().size() == 0) {
    location.add(MemoryLocationConstraint(true));
  }

  for (auto &parentPtr : parents) {
    auto parent = parentPtr.lock();
    if (parent) {
      location = parent->getLocation().merge(location);
    }
  }

  hasCachedLocation = true;
  return location;
}

// ----------------------------------------------------------------------------
const MemoryLocationList &PlacerNode::getShadowedLocation() {
  /*if (hasCachedShadowedLocation) {
    return shadowedLocation;
  }

  shadowedLocation = getLocation().applyMemoryShadows(placer->getMemoryMap());

  hasCachedShadowedLocation = true;
  return shadowedLocation;*/
  return getLocation();
}

// ----------------------------------------------------------------------------
bool PlacerNode::collectValidPlacements() {
  if (!requiresPlacement() || hasCachedValidLocations) {
    return true;
  }

  hasCachedValidLocations = true;
  const auto &locations = getShadowedLocation();
  validLocations = getMemoryMap()->filterValidLocations(locations, node->getSize());

# ifdef DEBUG_FMA
    if (!validLocations.getAllowed().size()) {
      placer->getProject()->log().trace() << "Placer: No possible location has been found for " << node->getNameHint();
    } else {
      placer->getProject()->log().trace() << "Placer: Found " << validLocations.getAllowed().size() << " possible location(s)";
    }
# endif

  return !!validLocations.getAllowed().size();
}

// ----------------------------------------------------------------------------
uint32_t PlacerNode::getNumValidPlacements() {
  return validLocations.getAllowed().size();
}

// ----------------------------------------------------------------------------
bool PlacerNode::place(uint32_t placement) {
  const auto &location = validLocations.getAllowed().at(placement);

# ifdef DEBUG_FMA
    placer->getProject()->log().trace() << "Placer: Place location " << location.asString();
# endif
  
  if (!getMemoryMap()->place(location, node->getSize())) {
#   ifdef DEBUG_FMA
      placer->getProject()->log().trace() << "Placer: Placement returned FALSE for location";
#   endif
    return false;
  }

  node->placement()->set(location.banks().front(), location.addresses().front(), node->getSize());
  
  return true;
}

// ----------------------------------------------------------------------------
bool PlacerNode::isMemberOf(const PlacerNodePtr &other) {
  if (other == shared_from_this()) {
    return true;
  }

  getChildren();
  for (auto &child : parents) {
    auto childPtr = child.lock();
    if (childPtr && childPtr->isMemberOf(other)) {
      return true;
    }
  }

  return false;
}

// ----------------------------------------------------------------------------
const PlacerMemoryMapPtr &PlacerNode::getMemoryMap() {
  if (parents.size() == 0) {
    return state->getMemoryMap();
  } else if (parents.size() == 1) {
    const auto &it = parents.front().lock();
    if (it) {
      return it->getMemoryMap();
    }
  }

  placer->getProject()->log().error() << "Declarations might not have more than one parent. Consider using a containing scope.";
  return state->getMemoryMap();
}

// ----------------------------------------------------------------------------
