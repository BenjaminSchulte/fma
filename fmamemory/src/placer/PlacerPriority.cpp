#include <placer/Placer.hpp>
#include <placer/PlacerPriority.hpp>

using namespace FMA::placer;

#define LEFT_FIRST true
#define RIGHT_FIRST false

bool __sortMemberNodes(const PlacerNodePtr &outer, const PlacerNodePtr &, bool &);

// ----------------------------------------------------------------------------
bool __sortMemberNodes(const PlacerNodePtr &outer, const PlacerNodePtr &, bool &) {
  if (outer->hasStaticAddress()) {
    return true;
  }

  return false;
}

// ----------------------------------------------------------------------------
bool FMA::placer::__sortNodes(const PlacerNodePtr &left, const PlacerNodePtr &right) {
  bool result;
  
  // Ignore one if placement is not necessary
  if (!left->requiresPlacement()) {
    return RIGHT_FIRST;
  } else if (!right->requiresPlacement()) {
    return LEFT_FIRST;
  }

  // Child/Parent scopes might have priority
  if (left->isMemberOf(right)) {
    if (__sortMemberNodes(right, left, result)) {
      return LEFT_FIRST;
    }
  } else if (right->isMemberOf(left)) {
    if (__sortMemberNodes(left, right, result)) {
      return !RIGHT_FIRST;
    }
  }

  // Static addresses have priority
  if (left->hasStaticAddress() && !right->hasStaticAddress()) {
    return LEFT_FIRST;
  } else if (right->hasStaticAddress() && !left->hasStaticAddress()) {
    return RIGHT_FIRST;
  }

  // Static banks have priority
  if (left->hasStaticBank() && !right->hasStaticBank()) {
    return LEFT_FIRST;
  } else if (right->hasStaticBank() && !left->hasStaticBank()) {
    return RIGHT_FIRST;
  }

  return left->score() > right->score();
}