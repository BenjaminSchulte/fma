#include <sstream>
#include <fma/ast/Expression.hpp>
#include <fma/core/Array.hpp>
#include <fma/types/Base.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/ParameterList.hpp>

using namespace FMA::ast;
using namespace FMA::core;
using namespace FMA::types;
using namespace FMA::interpret;

// ----------------------------------------------------------------------------
ArrayExpression::ArrayExpression(const ast::ParameterList &items)
  : items(items)
{
}

// ----------------------------------------------------------------------------
std::string ArrayExpression::asString(const std::string &) const {
  std::ostringstream os;
  os << "[ARRAY]";
  return os.str();
}

// ---------------------------------------------------------------------------
ResultPtr ArrayExpression::execute(const ContextPtr &context) const {
  interpret::GroupedParameterList params = interpret::GroupedParameterList::createFromAstParameters(context, items);

  std::vector<TypePtr> values;
  for (auto &item:params.only_args()) {
    values.push_back(item);
  }

  return ArrayClass::createInstance(context, values);
}

// ----------------------------------------------------------------------------