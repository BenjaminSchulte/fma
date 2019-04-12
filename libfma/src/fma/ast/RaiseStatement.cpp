#include <sstream>
#include <fma/ast/Expression.hpp>
#include <fma/ast/Statement.hpp>
#include <fma/ast/Identifier.hpp>
#include <fma/types/Undefined.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/Project.hpp>

using namespace FMA::ast;
using namespace FMA::interpret;
using namespace FMA::types;

#include <iostream>

// ----------------------------------------------------------------------------
RaiseStatement::RaiseStatement()
  : expression()
{
}
// ----------------------------------------------------------------------------
RaiseStatement::RaiseStatement(Expression *expression)
  : expression(expression)
{
}

// ----------------------------------------------------------------------------
RaiseStatement::~RaiseStatement() {
}

// ----------------------------------------------------------------------------
std::string RaiseStatement::asString(const std::string &prefix) const {
  std::ostringstream os;
  os << prefix << "raise " << expression->asString("") << std::endl;
  if (nextStatement) { os << nextStatement->asString(prefix); }
  return os.str();
}

// ---------------------------------------------------------------------------
ResultPtr RaiseStatement::execute(const ContextPtr &context) const {
  ResultPtr result = context->execute(expression);
  std::string error = result->get()->convertToString(context);
  context->log().error() << error;
  
  return Result::returns(TypePtr(new Undefined()));
}
