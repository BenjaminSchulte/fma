#include <sstream>
#include <fma/ast/Expression.hpp>
#include <fma/ast/Statement.hpp>
#include <fma/ast/Identifier.hpp>
#include <fma/types/Base.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/ObjectContext.hpp>
#include <fma/interpret/ParameterList.hpp>

using namespace FMA::ast;
using namespace FMA::types;
using namespace FMA::interpret;

// ----------------------------------------------------------------------------
CaseStatement::CaseStatement(Expression *expression, const CaseStatementCaseList &cases)
  : expression(expression)
  , cases(cases)
{
}

// ----------------------------------------------------------------------------
std::string CaseStatement::asString(const std::string &prefix) const {
  std::ostringstream os;
  os << prefix << "case " << expression->asString("") << std::endl;
  os << prefix << "end" << std::endl;
  if (nextStatement) { os << nextStatement->asString(prefix); }
  return os.str();
}

// ---------------------------------------------------------------------------
ResultPtr CaseStatement::execute(const ContextPtr &context) const {
  TypePtr compareValue = expression->execute(context)->get();
  
  ContextPtr callContext(new ObjectContext(context->getInterpreter(), compareValue));
  TypePtr eq = compareValue->getMember("==");
  StatementPtr callStatements;

  for (auto &item : cases) {
    if (!item.expression) {
      callStatements = item.statement;
      continue;
    }

    TypePtr caseValue = item.expression->execute(context)->get();
    interpret::GroupedParameterList params;
    params.push_back(caseValue);

    if (eq->call(callContext, params)->get()->convertToBoolean(context)) {
      callStatements = item.statement;
      break;
    }
  }

  if (callStatements) {
    return context->execute(callStatements);
  } else {
    return ResultPtr(new Result());
  }
}
