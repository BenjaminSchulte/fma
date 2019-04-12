#include <sstream>
#include <fma/ast/Expression.hpp>
#include <fma/core/String.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Result.hpp>

using namespace FMA::ast;
using namespace FMA::core;
using namespace FMA::interpret;

// ----------------------------------------------------------------------------
StringLiteral::StringLiteral(const std::string &string)
  : string(string)
{
}

// ----------------------------------------------------------------------------
std::string StringLiteral::asString(const std::string &) const {
  std::ostringstream os;
  os << "\"" << string << "\"";
  return os.str();
}

// ---------------------------------------------------------------------------
ResultPtr StringLiteral::execute(const ContextPtr &context) const {
  return StringClass::createInstance(context, string);
}

// ----------------------------------------------------------------------------