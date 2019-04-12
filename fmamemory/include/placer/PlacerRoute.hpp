#ifndef __PLACER_PLACERROUTE_H__
#define __PLACER_PLACERROUTE_H__

#include <vector>
#include "PlacerStatePointer.hpp"
#include "PlacerNode.hpp"

namespace FMA {
namespace placer {

class PlacerRoute {
public:
  PlacerRoute();
  virtual ~PlacerRoute();

  void selectState(const PlacerStatePtr &_state);
  void rewind();

  bool validNext();
  uint32_t next();
  bool more();
  void fail();

protected:
  PlacerStatePtr state;
  PlacerNodeList::const_iterator node;
  uint32_t _iteration;
  uint32_t _current;
  uint32_t _next;
  bool lastOverflow;
  bool overflow;
};

}
}

#endif
