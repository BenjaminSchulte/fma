#include <sstream>
#include <fma/ast/Expression.hpp>
#include <fma/Project.hpp>
#include <fma/types/Base.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/ObjectContext.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/interpret/Interpreter.hpp>

using namespace FMA::ast;
using namespace FMA::interpret;

// ----------------------------------------------------------------------------
CallExpression::CallExpression(Expression *expression)
  : expression(expression)
{
}

// ----------------------------------------------------------------------------
std::string CallExpression::asString(const std::string &) const {
  std::ostringstream os;
  os << "<CALL " << expression->asString("");
  bool isFirst = true;
  os << "(";
  for (auto && item : arguments) {
    if (isFirst) {
      isFirst = false;
    } else {
      os << ", ";
    }

    os << item->asString("");
  }
  os << ")>";
  return os.str();
}

// ---------------------------------------------------------------------------
ResultPtr CallExpression::execute(const ContextPtr &context) const {
  ResultPtr callback = context->execute(expression);
  callback->pretendExecuted();
  
  if (callback->isUndefined()) {
    context->log().error() << location << "Call to undefined function: " << expression->asString("");
    return ResultPtr(new Result());
  }

  ContextPtr callContext;
  if (callback->hasContextObject()) {
    callContext = ContextPtr(new ObjectContext(context->getInterpreter(), callback->getContextObject()));
  } else {
    callContext = ContextPtr(new BaseContext(context->getInterpreter()));
  }

  interpret::GroupedParameterList parameters = interpret::GroupedParameterList::createFromAstParameters(context, arguments);

  callContext->setCallee(context->getDeclareLevelObject());

  auto _stack = context->getInterpreter()->stack().enter(location);
  ResultPtr result = callback->get()->call(callContext, parameters);
  result->pretendExecuted();

  return result;
}


// ----------------------------------------------------------------------------
