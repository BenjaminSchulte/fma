#include <placer/PlacerRoute.hpp>
#include <iostream>

using namespace FMA::placer;

// ----------------------------------------------------------------------------
PlacerRoute::PlacerRoute()
  : _iteration(0)
  , overflow(false)
{
}

// ----------------------------------------------------------------------------
PlacerRoute::~PlacerRoute() {
}

// ----------------------------------------------------------------------------
void PlacerRoute::selectState(const PlacerStatePtr &_state) {
  state = _state;
}

// ----------------------------------------------------------------------------
void PlacerRoute::rewind() {
  node = state->getNodes().begin();
  _current = _iteration++;
  lastOverflow = false;
}

// ----------------------------------------------------------------------------
bool PlacerRoute::validNext() {
  if (node == state->getNodes().end()) {
    return false;
  }

  PlacerNode *cur = (node++)->get();
  if (!cur->requiresPlacement()) {
    return true;
  }

  cur->collectValidPlacements();
  uint32_t maxNum = cur->getNumValidPlacements();
  if (!maxNum) {
    return false;
  }

  if (maxNum == 1) {
    _next = 0;
  } else {
    _next = _current % maxNum;
    _current /= maxNum;
  }
  
  return true;
}

// ----------------------------------------------------------------------------
void PlacerRoute::fail() {
}

// ----------------------------------------------------------------------------
uint32_t PlacerRoute::next() {
  return _next;
}

// ----------------------------------------------------------------------------
bool PlacerRoute::more() {
  return !state.valid() || !overflow;
}

