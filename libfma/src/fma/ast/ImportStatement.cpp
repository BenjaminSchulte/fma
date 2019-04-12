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
ImportStatement::ImportStatement(FMA::ast::Identifier *identifier)
  : identifier(identifier)
{
}

// ----------------------------------------------------------------------------
ImportStatement::~ImportStatement() {
}

// ----------------------------------------------------------------------------
std::string ImportStatement::asString(const std::string &prefix) const {
  std::ostringstream os;
  os << prefix << "import " << identifier->asString("") << std::endl;
  if (nextStatement) { os << nextStatement->asString(prefix); }
  return os.str();
}

// ---------------------------------------------------------------------------
ResultPtr ImportStatement::execute(const ContextPtr &context) const {
  std::string name = identifier->asString("");

  SymbolReferencePtr ref(new SymbolReference(name));
  context->setMember(name, SymbolReferenceClass::createInstance(context, ref)->get());

  return Result::executed(context, undefinedValue);
}
