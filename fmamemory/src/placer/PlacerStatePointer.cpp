#include <placer/Placer.hpp>
#include <placer/PlacerStatePointer.hpp>

using namespace FMA::placer;

// ----------------------------------------------------------------------------
PlacerStatePtr::PlacerStatePtr(PlacerState *state)
  : state(state)
{
}

// ----------------------------------------------------------------------------
PlacerStatePtr::~PlacerStatePtr() {
}

// ----------------------------------------------------------------------------
