#include <memory/ArrayMemberClassInstance.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <sstream>
#include <iostream>

using namespace FMA::memory;
using namespace FMA::types;
using namespace FMA::interpret;

// ----------------------------------------------------------------------------
ArrayMemberClassInstance::ArrayMemberClassInstance(const ContextPtr &context, const TypePtr &callback)
  : context(context)
  , callback(callback)
{
}

// ----------------------------------------------------------------------------
std::string ArrayMemberClassInstance::asString() const {
  std::ostringstream os;
  os << "<ArrayMemberClassInstance " << callback->asString() << ">";
  return os.str();
}

// ----------------------------------------------------------------------------
ResultPtr ArrayMemberClassInstance::callWithoutDecoratorTest(const ContextPtr &, const GroupedParameterList &params) {
  return callback->call(context, params);
}