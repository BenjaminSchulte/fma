#include <sstream>
#include <fma/Project.hpp>
#include <fma/ast/Identifier.hpp>
#include <fma/ast/Expression.hpp>
#include <fma/types/Base.hpp>
#include <fma/core/Nil.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/ObjectContext.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/Interpreter.hpp>

using namespace FMA::ast;
using namespace FMA::interpret;
using namespace FMA::types;
using namespace FMA::core;

// ----------------------------------------------------------------------------
Identifier::Identifier(const std::string &name)
  : name(name)
  , root(false)
  , nextIdentifier(NULL)
{
}

// ----------------------------------------------------------------------------
Expression *Identifier::createStringExpression() const {
  return new StringLiteral(name);
}

// ----------------------------------------------------------------------------
void Identifier::linkLastStatement(Identifier *next) {
  Identifier* current = this;

  while (current->nextIdentifier) {
    current = current->nextIdentifier.get();
  }

  current->nextIdentifier = IdentifierPtr(next);
}

// ----------------------------------------------------------------------------
std::string Identifier::asString(const std::string &) const {
  if (!nextIdentifier) {
    return name;
  }

  std::ostringstream os;
  os << name;

  const Identifier* current = nextIdentifier.get();
  while (current) {
    os << "::" << current->name;

    current = current->nextIdentifier.get();
  }

  return os.str();
}

// ---------------------------------------------------------------------------
ResultPtr Identifier::execute(const ContextPtr &context) const {
  if (name == "__DUMP_CURRENT_CONTEXT") {
    context->log().warn() << "Current context: " << context->asString();
    return NilClass::createInstance(context);
  }

  const Identifier* currentIdentifier = this;
  ContextPtr current = context;
  ResultPtr result;
  if (root) {
    result = current->getRootLevelContext()->getMember(name);
  } else {
    result = current->resolve(name);
  }

  if (result->isUndefined()) {
    ContextPtr global = current->getInterpreter()->getGlobalContext();

    if (global) {
      ResultPtr newResult = global->resolve("::" + name);
      if (!newResult->isUndefined()) {
        result = newResult;
      }
    }
  }

  result->resolvedAt(location);

  CodeReference ttt = location;
  while (currentIdentifier->nextIdentifier) {
    currentIdentifier = currentIdentifier->nextIdentifier.get();

    TypePtr type = result->get();
    result = ResultPtr(new Result(context, type->getDirectMember(currentIdentifier->name)));
    result->makeAssignable(ContextPtr(new ObjectContext(context->getInterpreter(), type)), currentIdentifier->name);
    result->resolvedAt(location);
  }
  
  return result;
}

// ----------------------------------------------------------------------------
