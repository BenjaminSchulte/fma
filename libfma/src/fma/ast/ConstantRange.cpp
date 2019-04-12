#include <sstream>
#include <fma/ast/Expression.hpp>
#include <fma/core/Range.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Result.hpp>

using namespace FMA::ast;
using namespace FMA::core;
using namespace FMA::interpret;

// ----------------------------------------------------------------------------
ConstantRange::ConstantRange(Expression *first, Expression *last)
  : first(first)
  , last(last)
{
}

// ----------------------------------------------------------------------------
std::string ConstantRange::asString(const std::string &) const {
  std::ostringstream os;
  os << first->asString("") << ".." << last->asString("");
  return os.str();
}

// ---------------------------------------------------------------------------
ResultPtr ConstantRange::execute(const ContextPtr &context) const {
  int64_t firstResult = context->execute(first)->get()->convertToNumber(context);
  int64_t lastResult = context->execute(last)->get()->convertToNumber(context);

  return RangeClass::createInstance(context, firstResult, lastResult);
}

// ----------------------------------------------------------------------------