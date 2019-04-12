#include <sstream>
#include <fma/ast/Expression.hpp>
#include <fma/ast/Identifier.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Result.hpp>

using namespace FMA::ast;
using namespace FMA::interpret;

#include <iostream>
#include <sstream>

// ----------------------------------------------------------------------------
IdentifierExpression::IdentifierExpression(Identifier *identifier)
  : identifier(identifier)
{
}

// ----------------------------------------------------------------------------
std::string IdentifierExpression::asString(const std::string &) const {
  std::ostringstream os;
  os << "<IdentifierExpression " << identifier->asString("") << ">"; 
  return os.str();
}

// ---------------------------------------------------------------------------
ResultPtr IdentifierExpression::execute(const ContextPtr &context) const {
  return identifier->execute(context);
}

