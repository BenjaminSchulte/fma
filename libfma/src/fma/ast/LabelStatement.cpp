#include <sstream>
#include <fma/ast/Expression.hpp>
#include <fma/ast/Statement.hpp>
#include <fma/ast/Identifier.hpp>
#include <fma/core/SymbolReference.hpp>
#include <fma/types/Base.hpp>
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
LabelStatement::LabelStatement(FMA::ast::Identifier *identifier)
  : identifier(identifier)
{
}

// ----------------------------------------------------------------------------
LabelStatement::~LabelStatement() {
}

// ----------------------------------------------------------------------------
std::string LabelStatement::asString(const std::string &prefix) const {
  std::ostringstream os;
  os << prefix << identifier->asString("") << ":" << std::endl;
  if (nextStatement) { os << nextStatement->asString(prefix); }
  return os.str();
}

// ---------------------------------------------------------------------------
void LabelStatement::registerLabels(const interpret::ContextPtr &context, const std::string &name, const StatementPtr &statement) {
  StatementPtr current(statement);

  for (; current; current=std::dynamic_pointer_cast<Statement>(current->getNextSibling())) {
    if (current->getNodeName() != "LabelStatement") {
      continue;
    }

    std::dynamic_pointer_cast<LabelStatement>(current)->registerLabel(context, name);
  }
}

// ---------------------------------------------------------------------------
void LabelStatement::registerLabel(const interpret::ContextPtr &context, const std::string &name) {
  const auto &ref = context->getProject()->getMemoryAdapter()->getSymbolMap()->createReference(name + "." + identifier->getName());

  const auto &member = context->resolve(identifier->getName());
  if (!member->isUndefined() && member->get()->isObjectOfType("SymbolReference")) {
    return;
  }
  
  context->setMember(identifier->getName(), SymbolReferenceClass::createInstance(context, ref)->get());
}

// ---------------------------------------------------------------------------
ResultPtr LabelStatement::execute(const ContextPtr &context) const {
  TypePtr sym;
  TypePtr symbol = context->resolve(identifier->getName())->get();
  if (symbol->isObjectOfType("SymbolReference")) {
    sym = symbol;
  }
  
  if (!sym) {
    context->log().error() << "Unable to find label";
    return ResultPtr(new Result());
  }

  ContextPtr global = context->getInterpreter()->getGlobalContext();
  if (!global) {
    context->log().error() << "No global context";
    return ResultPtr(new Result());
  }

  ResultPtr writeLabel = global->resolve("::label");
  if (writeLabel->isUndefined()) {
    context->log().error() << "Missing .label callback in datablock";
    return ResultPtr(new Result());
  }

  interpret::GroupedParameterList params;
  params.push_back(sym);
  writeLabel->pretendExecuted();
  return writeLabel->get()->call(global, params);
}
