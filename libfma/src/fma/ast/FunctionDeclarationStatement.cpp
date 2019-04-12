#include <sstream>
#include <fma/ast/Expression.hpp>
#include <fma/ast/Statement.hpp>
#include <fma/ast/Identifier.hpp>
#include <fma/core/Function.hpp>
#include <fma/types/Macro.hpp>
#include <fma/types/DecoratorContainer.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/ObjectContext.hpp>
#include <fma/interpret/NameHintContext.hpp>

using namespace FMA::ast;
using namespace FMA::interpret;
using namespace FMA::types;

// ----------------------------------------------------------------------------
FunctionDeclarationStatement::FunctionDeclarationStatement(Identifier *identifier, Statement *children, bool exported)
  : identifier(identifier)
  , children(children)
  , exported(exported)
{
}

// ----------------------------------------------------------------------------
ResultPtr FunctionDeclarationStatement::execute(const ContextPtr &context) const {
  ContextPtr current(context->getDeclareLevelContext());

  IdentifierPtr identifier = this->identifier;
  const std::string &name = identifier->getName();

  ast::MacroParameterList macroParameters;
  Macro *macroObj = new Macro(name, current, macroParameters, children);
  MacroPtr macro(macroObj);
  attachDecoratable(context, macroObj);

  ContextPtr createContext(new NameHintContext(context->getInterpreter(), name, context));
  ResultPtr result = core::FunctionClass::createInstance(createContext, macro);

  result = wrapDecoratable(context, result);

  ResultPtr member = current->getMember(name);
  member->assign(result->get());

  TypePtr parentDecoratorContainer = current->resolve(FMA_TYPE_DECORATORCONTAINER_MEMBER)->get();
  if (parentDecoratorContainer->isDecoratorContainer()) {
    result->get()->setMember(
      FMA_TYPE_DECORATORCONTAINER_MEMBER,
      std::dynamic_pointer_cast<DecoratorContainer>(parentDecoratorContainer)->createChild()
    );
  }

  ContextPtr funcContext(new ObjectContext(context->getInterpreter(), result->get()));
  LabelStatement::registerLabels(funcContext, createContext->getNameHint(), children);

  if (exported) {
    interpret::GroupedParameterList empty;
    result->get()->call(funcContext, empty);
  }

  return result;
}

// ----------------------------------------------------------------------------
std::string FunctionDeclarationStatement::asString(const std::string &prefix) const {
  std::ostringstream os;
  os << prefix << "def " << identifier->asString("") << std::endl;
  if (children) {
    os << children->asString(prefix + "  ");
  }
  os << prefix << "end" << std::endl;
  if (nextStatement) { os << nextStatement->asString(prefix); }
  return os.str();
}
