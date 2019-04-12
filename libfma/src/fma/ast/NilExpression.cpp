#include <sstream>
#include <fma/ast/Expression.hpp>
#include <fma/core/Nil.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Result.hpp>

using namespace FMA::ast;
using namespace FMA::core;
using namespace FMA::interpret;

// ----------------------------------------------------------------------------
NilExpression::NilExpression()
{
}

// ----------------------------------------------------------------------------
std::string NilExpression::asString(const std::string &) const {
  return "nil";
}

// ---------------------------------------------------------------------------
ResultPtr NilExpression::execute(const ContextPtr &context) const {
  return NilClass::createInstance(context);
}

// ----------------------------------------------------------------------------