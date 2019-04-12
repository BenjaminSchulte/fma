#ifndef __FMA_PLACER_PLACERSTATEPOINTER_H__
#define __FMA_PLACER_PLACERSTATEPOINTER_H__

#include "PlacerState.hpp"

namespace FMA {
namespace placer {

class PlacerStatePtr {
public:
  PlacerStatePtr() : state(NULL) {}
  PlacerStatePtr(PlacerState *state);
  ~PlacerStatePtr();

  PlacerState *operator->() const { return state; }
  bool valid() const { return !!state; }

protected:
  PlacerState *state;
};

}
}

#endif
