#include <sstream>
#include <fma/Project.hpp>
#include <fma/ast/Statement.hpp>
#include <fma/ast/Identifier.hpp>
#include <fma/ast/Expression.hpp>
#include <fma/types/Decoratable.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/Interpreter.hpp>

using namespace FMA::ast;
using namespace FMA::types;
using namespace FMA::interpret;

// ----------------------------------------------------------------------------
void DecoratableStatement::attachDecoratable(const ContextPtr &context, Decoratable *object) const {
  if (!decorator) {
    return;
  }

  ResultPtr macro = decorator->execute(context);
  object->setDecorator(macro->get());
}

// ----------------------------------------------------------------------------
ResultPtr DecoratableStatement::wrapDecoratable(const ContextPtr &, const ResultPtr &result) const {
  return result;
}

// ----------------------------------------------------------------------------
