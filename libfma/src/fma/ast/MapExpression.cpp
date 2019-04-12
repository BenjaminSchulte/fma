#include <sstream>
#include <fma/ast/Expression.hpp>
#include <fma/core/Map.hpp>
#include <fma/types/Base.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/ParameterList.hpp>

using namespace FMA::ast;
using namespace FMA::core;
using namespace FMA::types;
using namespace FMA::interpret;

// ----------------------------------------------------------------------------
MapExpression::MapExpression(const ast::ParameterList &items)
  : items(items)
{
}

// ----------------------------------------------------------------------------
std::string MapExpression::asString(const std::string &) const {
  std::ostringstream os;
  os << "{MAP}";
  return os.str();
}

// ---------------------------------------------------------------------------
ResultPtr MapExpression::execute(const ContextPtr &context) const {
  interpret::GroupedParameterList params = interpret::GroupedParameterList::createFromAstParameters(context, items);

  std::map<std::string, TypePtr> values;
  for (auto &item:params.only_kwargs()) {
    values[item.first] = item.second;
  }

  return MapClass::createInstance(context, values);
}

// ----------------------------------------------------------------------------