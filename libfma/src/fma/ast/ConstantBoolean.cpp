#include <sstream>
#include <fma/ast/Expression.hpp>
#include <fma/core/Boolean.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Result.hpp>

using namespace FMA::ast;
using namespace FMA::core;
using namespace FMA::interpret;

// ----------------------------------------------------------------------------
ConstantBoolean::ConstantBoolean(const uint8_t number)
  : number(number)
{
}

// ----------------------------------------------------------------------------
std::string ConstantBoolean::asString(const std::string &) const {
  if (number) {
    return "true";
  } else {
    return "false";
  }
  std::ostringstream os;
  os << number;
  return os.str();
}

// ---------------------------------------------------------------------------
ResultPtr ConstantBoolean::execute(const ContextPtr &context) const {
  return BooleanClass::createInstance(context, !!number);
}

// ----------------------------------------------------------------------------