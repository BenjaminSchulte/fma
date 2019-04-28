#include <fma/types/Class.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <sstream>

using namespace FMA::types;
using namespace FMA::interpret;

// ----------------------------------------------------------------------------
ClassPtr ClassPrototype::getClass() {
  return klass.lock();
}

// ----------------------------------------------------------------------------
std::string ClassPrototype::asString() const {
  std::ostringstream os;
  ClassPtr klassPtr = klass.lock();

  if (klassPtr) {
    os << "<ClassPrototype " << klassPtr->getName() << ">";
  } else {
    os << "<ClassPrototype>";
  }

  return os.str();
}

// ----------------------------------------------------------------------------

