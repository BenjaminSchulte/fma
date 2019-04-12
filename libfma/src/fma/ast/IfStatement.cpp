#include <sstream>
#include <fma/ast/Expression.hpp>
#include <fma/ast/Statement.hpp>
#include <fma/ast/Identifier.hpp>
#include <fma/types/Base.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/BaseContext.hpp>

using namespace FMA::ast;
using namespace FMA::interpret;

// ----------------------------------------------------------------------------
IfStatement::IfStatement(Expression *expression)
  : expression(expression)
{
}

// ----------------------------------------------------------------------------
std::string IfStatement::asString(const std::string &prefix) const {
  std::ostringstream os;
  os << prefix << "if " << expression->asString("") << std::endl;
  if (thenBranch) {
    os << thenBranch->asString(prefix + "  ");
  }
  if (elseBranch) {
    os << prefix << "else" << std::endl;
    os << elseBranch->asString(prefix + "  ");
  }
  os << prefix << "end" << std::endl;
  if (nextStatement) { os << nextStatement->asString(prefix); }
  return os.str();
}

// ---------------------------------------------------------------------------
ResultPtr IfStatement::execute(const ContextPtr &context) const {
  bool result = expression->execute(context)->get()->convertToBoolean(context);
  ResultPtr lastValue;

  if (result) {
    if (thenBranch) {
      lastValue = context->execute(thenBranch);
    } else {
      lastValue = ResultPtr(new Result());
    }
  } else {
    if (elseBranch) {
      lastValue = context->execute(elseBranch);
    } else {
      lastValue = ResultPtr(new Result());
    }
  }

  return lastValue;
}
