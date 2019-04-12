#include <sstream>
#include <fma/Project.hpp>
#include <fma/ast/Statement.hpp>
#include <fma/ast/Identifier.hpp>
#include <fma/ast/Expression.hpp>
#include <fma/types/Block.hpp>
#include <fma/types/Decorator.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/Interpreter.hpp>
#include <fma/interpret/ObjectContext.hpp>
#include <fma/interpret/NestedContext.hpp>

using namespace FMA::ast;
using namespace FMA::types;
using namespace FMA::interpret;

// ----------------------------------------------------------------------------
DecoratorCallStatement::DecoratorCallStatement(Identifier *identifier, const ast::ParameterList &parameters)
  : identifier(identifier)
  , parameters(parameters)
{
}

// ----------------------------------------------------------------------------
ResultPtr DecoratorCallStatement::execute(const ContextPtr &context) const {
  ResultPtr macro = identifier->execute(context);
  if (macro->isUndefined()) {
    return ResultPtr(new Result());
  }

  macro->pretendExecuted();

  interpret::GroupedParameterList parsedParameters = interpret::GroupedParameterList::createFromAstParameters(context, parameters);
  Decorator *decorator = new Decorator(macro->get(), parsedParameters);

  attachDecoratable(context, decorator);

  return Result::executed(context, TypePtr(decorator));
}

// ----------------------------------------------------------------------------
