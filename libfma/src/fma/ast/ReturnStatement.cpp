#include <sstream>
#include <fma/ast/Expression.hpp>
#include <fma/ast/Statement.hpp>
#include <fma/ast/Identifier.hpp>
#include <fma/types/Undefined.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Result.hpp>

using namespace FMA::ast;
using namespace FMA::interpret;
using namespace FMA::types;

#include <iostream>

// ----------------------------------------------------------------------------
ReturnStatement::ReturnStatement()
  : expression()
{
}
// ----------------------------------------------------------------------------
ReturnStatement::ReturnStatement(Expression *expression)
  : expression(expression)
{
}

// ----------------------------------------------------------------------------
ReturnStatement::~ReturnStatement() {
}

// ----------------------------------------------------------------------------
std::string ReturnStatement::asString(const std::string &prefix) const {
  std::ostringstream os;
  os << prefix << "return " << expression->asString("") << std::endl;
  if (nextStatement) { os << nextStatement->asString(prefix); }
  return os.str();
}

// ---------------------------------------------------------------------------
ResultPtr ReturnStatement::execute(const ContextPtr &context) const {
  if (!expression) {
    return Result::returns(TypePtr(new Undefined()));
  }

  ResultPtr result = context->execute(expression);
  return Result::returns(result->get());
}
