#include <fma/ast/Expression.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/BaseContext.hpp>

using namespace FMA::ast;
using namespace FMA::interpret;

// ---------------------------------------------------------------------------
ResultPtr Parameter::execute(const ContextPtr &context) const {
  return value->execute(context);
}

// ---------------------------------------------------------------------------
std::string Parameter::asString(const std::string &) const {
  switch (type) {
  case Parameter::ARG:
    return value->asString("");

  default:
    return "#PAR";
  }
}

// ----------------------------------------------------------------------------
