#include <fma/types/Undefined.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/Project.hpp>

using namespace FMA;
using namespace FMA::types;
using namespace FMA::interpret;

namespace FMA {
namespace types {
TypePtr undefinedValue(new Undefined());
}
}

// ----------------------------------------------------------------------------
bool Undefined::isUndefined() const {
  return true;
}

// ----------------------------------------------------------------------------
std::string Undefined::asString() const {
  return "<Undefined>";
}

// ----------------------------------------------------------------------------
ResultPtr Undefined::callWithoutDecoratorTest(const ContextPtr &context, const GroupedParameterList &) {
  context->log().error() << "Expression result is <Undefined>";

  return Result::executed(context, getPointer());
}

// ----------------------------------------------------------------------------
