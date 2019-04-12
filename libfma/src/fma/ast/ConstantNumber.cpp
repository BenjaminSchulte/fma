#include <sstream>
#include <fma/ast/Expression.hpp>
#include <fma/core/Number.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Result.hpp>

using namespace FMA::ast;
using namespace FMA::core;
using namespace FMA::interpret;

// ----------------------------------------------------------------------------
ConstantNumber::ConstantNumber(const int64_t number)
  : number(number)
{
}

// ----------------------------------------------------------------------------
std::string ConstantNumber::asString(const std::string &) const {
  std::ostringstream os;
  os << number;
  return os.str();
}

// ---------------------------------------------------------------------------
ResultPtr ConstantNumber::execute(const ContextPtr &context) const {
  return NumberClass::createInstance(context, number);
}

// ----------------------------------------------------------------------------