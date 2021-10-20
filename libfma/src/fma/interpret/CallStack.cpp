#include <fma/interpret/CallStack.hpp>

using namespace FMA;
using namespace FMA::interpret;

// ----------------------------------------------------------------------------
CallStackContext::~CallStackContext() {
  stack->leave();
}