#include <fma/types/InternalValue.hpp>
#include <fma/types/Class.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <sstream>
#include <iostream>

using namespace FMA;
using namespace FMA::types;
using namespace FMA::interpret;

namespace FMA {
namespace types {
  template<> const char *getInternalObjectName<std::vector<TypePtr> >() { return "Array"; }
  template<> const char *getInternalObjectName<std::map<std::string, TypePtr> >() { return "Map"; }
  template<> const char *getInternalObjectName<std::string>() { return "String"; }
  template<> const char *getInternalObjectName<long double>() { return "Number"; }
  template<> const char *getInternalObjectName<symbol::ReferencePtr>() { return "SymbolReference"; }
  template<> const char *getInternalObjectName<output::DynamicBufferPtr>() { return "DynamicBuffer"; }
  template<> const char *getInternalObjectName<serialize::WeakSerializeObjectPtr>() { return "SerializeObject"; }
  template<> const char *getInternalObjectName<serialize::WeakDeserializeObjectPtr>() { return "DeserializeObject"; }
}
}

// ----------------------------------------------------------------------------
ResultPtr InternalFunctionValue::callInDecorator(const ContextPtr &context, const GroupedParameterList &parameter, DecoratorCallType) {
  return func(context, parameter);
}

// ----------------------------------------------------------------------------
std::string InternalFunctionValue::asString() const {
  std::ostringstream os;
  os << "<InternalFunction " << name << ">";
  return os.str();
}

// ----------------------------------------------------------------------------

