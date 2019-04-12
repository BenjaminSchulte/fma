#include <sstream>
#include <fma/ast/Expression.hpp>
#include <fma/ast/Statement.hpp>
#include <fma/ast/Identifier.hpp>
#include <fma/types/Macro.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/BaseContext.hpp>

using namespace FMA::ast;
using namespace FMA::interpret;
using namespace FMA::types;

// ----------------------------------------------------------------------------
InlineMacroExpression::InlineMacroExpression(const MacroParameterList &parameters, Statement *children)
  : children(children)
  , parameters(parameters)
{
}

// ----------------------------------------------------------------------------
ResultPtr InlineMacroExpression::execute(const ContextPtr &context) const {
  ContextPtr current(context->getDeclareLevelContext());
  
  TypePtr macro(new Macro("", current, parameters, children));
  ResultPtr result(new Result(context, macro));
  result->pretendExecuted();
  return result;
}

// ----------------------------------------------------------------------------
std::string InlineMacroExpression::asString(const std::string &prefix) const {
  std::ostringstream os;
  os << prefix << "do" << std::endl;
  if (children) {
    os << children->asString(prefix + "  ");
  }
  os << prefix << "end" << std::endl;
  return os.str();
}
