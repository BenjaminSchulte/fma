#include <sstream>
#include <fma/ast/Expression.hpp>
#include <fma/types/Base.hpp>
#include <fma/types/Pointer.hpp>
#include <fma/core/Boolean.hpp>
#include <fma/core/Number.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/Project.hpp>

using namespace FMA::ast;
using namespace FMA::core;
using namespace FMA::types;
using namespace FMA::interpret;

// ----------------------------------------------------------------------------
UnaryOperationExpression::UnaryOperationExpression(UnaryOperationExpression::Operator op, Expression *right)
  : op(op)
  , expression(right)
{
}

// ----------------------------------------------------------------------------
std::string UnaryOperationExpression::asString(const std::string &) const {
  std::ostringstream os;
  switch (op) {
    case NOT: os << "!"; break;
    case TILDE: os << "~"; break;

    default: os << "???"; break;
  }
  os << expression->asString("");
  return os.str();
}

// ---------------------------------------------------------------------------
ResultPtr UnaryOperationExpression::execute(const ContextPtr &context) const {
  ResultPtr value = context->execute(expression);
  ResultPtr result;

  if (value->isUndefined()) {
    context->log().error() << location << "Expression is undefined";
    return ResultPtr(new Result());
  }

  switch (op) {
  case NOT:
    result = BooleanClass::createInstance(context, !value->get()->convertToBoolean(context));
    break;

  case TILDE:
    result = NumberClass::createInstance(context, ~((uint64_t)value->get()->convertToNumber(context)));
    break;

  case POINTER:
    value->pretendExecuted();
    result = ResultPtr(new Result(context, TypePtr(new Pointer(value->get()))));
    break;

  default:
    std::cout << "TODO: Unhandled operator in UnaryOperationExpression "<< op << std::endl;
    result = value;
    break;
  }

  return result;
}

// ----------------------------------------------------------------------------
