#include <sstream>
#include <fma/ast/Expression.hpp>
#include <fma/ast/Statement.hpp>
#include <fma/ast/Identifier.hpp>
#include <fma/types/Base.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/ParameterList.hpp>

using namespace FMA::ast;
using namespace FMA::types;
using namespace FMA::interpret;

#include <iostream>

// ----------------------------------------------------------------------------
ExpressionStatement::ExpressionStatement(Expression *expression)
  : expression(expression)
{
}

// ----------------------------------------------------------------------------
ExpressionStatement::~ExpressionStatement() {
}

// ----------------------------------------------------------------------------
std::string ExpressionStatement::asString(const std::string &prefix) const {
  std::ostringstream os;
  os << prefix << expression->asString("") << std::endl;
  if (nextStatement) { os << nextStatement->asString(prefix); }
  return os.str();
}

// ---------------------------------------------------------------------------
ResultPtr ExpressionStatement::execute(const ContextPtr &context) const {
  ResultPtr result = context->execute(expression);
  TypePtr item = result->get();

  if (!item->isUndefined() && item->hasMember("!()")) {
    interpret::GroupedParameterList empty;
    result = item->callDirect("!()", context, empty);
  }

  return result;
}
