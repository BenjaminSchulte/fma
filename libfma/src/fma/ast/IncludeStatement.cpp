#include <sstream>
#include <fma/ast/Expression.hpp>
#include <fma/ast/Statement.hpp>
#include <fma/ast/Identifier.hpp>
#include <fma/core/SymbolReference.hpp>
#include <fma/types/Base.hpp>
#include <fma/types/Undefined.hpp>
#include <fma/plugin/MemoryPluginAdapter.hpp>
#include <fma/symbol/SymbolReference.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/Project.hpp>
#include <fma/interpret/Interpreter.hpp>

using namespace FMA::ast;
using namespace FMA::core;
using namespace FMA::interpret;
using namespace FMA::types;
using namespace FMA::symbol;

// ----------------------------------------------------------------------------
IncludeStatement::IncludeStatement(FMA::ast::Identifier *identifier)
  : identifier(identifier)
{
}

// ----------------------------------------------------------------------------
IncludeStatement::~IncludeStatement() {
}

// ----------------------------------------------------------------------------
std::string IncludeStatement::asString(const std::string &prefix) const {
  std::ostringstream os;
  os << prefix << "include " << identifier->asString("") << std::endl;
  if (nextStatement) { os << nextStatement->asString(prefix); }
  return os.str();
}

// ---------------------------------------------------------------------------
ResultPtr IncludeStatement::execute(const ContextPtr &context) const {
  ResultPtr result = context->execute(identifier);
  result->pretendExecuted();

  if (!result->isClass()) {
    context->log().error() << "Could not resolve identifier as class";
    return Result::executed(context, undefinedValue);
  }

  TypePtr self(context->getDeclareLevelObject());
  if (!self->isClass()) {
    context->log().error() << "'include' must be called within class context";
    return Result::executed(context, undefinedValue);
  }

  ClassPtr childKlass(result->asClass());
  ClassPtr parentKlass(self->asClass());

  parentKlass->extends(childKlass);
  
  return Result::executed(context, undefinedValue);
}
