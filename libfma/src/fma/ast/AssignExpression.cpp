#include <sstream>
#include <fma/ast/Expression.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/Project.hpp>

using namespace FMA::ast;
using namespace FMA::interpret;

// ----------------------------------------------------------------------------
AssignExpression::AssignExpression(Expression *left, Expression *right)
  : left(left)
  , right(right)
{
}

// ----------------------------------------------------------------------------
std::string AssignExpression::asString(const std::string &) const {
  std::ostringstream os;
  os << left->asString("");
  os << "=";
  os << right->asString("");
  return os.str();
}

// ---------------------------------------------------------------------------
ResultPtr AssignExpression::execute(const ContextPtr &context) const {
  ResultPtr lside;

  if (left->getNodeName() == "IdentifierExpression") {
    lside = ResultPtr(context->withoutInstances()->execute(left));
  } else {
    lside = ResultPtr(context->execute(left));
  }

  ResultPtr rside = context->execute(right);

  if (lside->isAssignable()) {
    lside->assign(rside->get());
    return lside;
  }
  
  context->log().error() << location << "Unable to assign variable";

  return ResultPtr(new Result());

}

// ----------------------------------------------------------------------------