#include <sstream>
#include <fma/ast/Expression.hpp>
#include <fma/core/ImmediateNumber.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Result.hpp>

using namespace FMA::ast;
using namespace FMA::interpret;
using namespace FMA::core;

// ----------------------------------------------------------------------------
ConstantExpression::ConstantExpression(Expression *expression)
  : expression(expression)
{
}

// ----------------------------------------------------------------------------
ResultPtr ConstantExpression::execute(const ContextPtr &context) const {
  return ImmediateNumberClass::createInstance(context, context->execute(expression)->get());
}

// ----------------------------------------------------------------------------
std::string ConstantExpression::asString(const std::string &) const {
  std::ostringstream os;
  os << "#" << expression->asString("") << std::endl;
  return os.str();
}

