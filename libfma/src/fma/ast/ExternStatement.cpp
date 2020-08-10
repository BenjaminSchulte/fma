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
ExternStatement::ExternStatement(FMA::ast::Identifier *identifier)
  : identifier(identifier)
{
}

// ----------------------------------------------------------------------------
ExternStatement::~ExternStatement() {
}

// ----------------------------------------------------------------------------
std::string ExternStatement::asString(const std::string &prefix) const {
  std::ostringstream os;
  os << prefix << "extern " << identifier->asString("") << std::endl;
  if (nextStatement) { os << nextStatement->asString(prefix); }
  return os.str();
}

// ---------------------------------------------------------------------------
ResultPtr ExternStatement::execute(const ContextPtr &context) const {
  ResultPtr lside = context->withoutInstances()->execute(identifier);

  if (!lside->isAssignable()) {
    context->log().error() << location << "Can not assign extern reference to variable";
    return ResultPtr(new Result());
  }

  SymbolReferencePtr ref(new SymbolReference(identifier->asString("")));
  lside->assign(SymbolReferenceClass::createInstance(context, ref)->get());

  return Result::executed(context, undefinedValue);
}
