#include <sstream>
#include <fma/ast/Expression.hpp>
#include <fma/core/Number.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Result.hpp>

using namespace FMA::ast;
using namespace FMA::core;
using namespace FMA::interpret;

// ----------------------------------------------------------------------------
ConstantNumber::ConstantNumber(const long double number)
  : number(number)
{
}

// ----------------------------------------------------------------------------
std::string ConstantNumber::asString(const std::string &) const {
  int64_t integerValue = number;
  std::ostringstream os;
  if (integerValue == number) {
    os << integerValue;
  } else {
    os << number;
  }
  return os.str();
}

// ---------------------------------------------------------------------------
ResultPtr ConstantNumber::execute(const ContextPtr &context) const {
  return NumberClass::createInstance(context, number);
}

// ----------------------------------------------------------------------------