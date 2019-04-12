#include <sstream>
#include <fma/ast/Expression.hpp>
#include <fma/ast/Statement.hpp>
#include <fma/ast/Identifier.hpp>
#include <fma/core/DataBlock.hpp>
#include <fma/types/Macro.hpp>
#include <fma/types/DecoratorContainer.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/NameHintContext.hpp>

using namespace FMA::ast;
using namespace FMA::interpret;
using namespace FMA::types;

// ----------------------------------------------------------------------------
DataBlockDeclarationStatement::DataBlockDeclarationStatement(Identifier *identifier, Statement *children)
  : identifier(identifier)
  , children(children)
{
}

// ----------------------------------------------------------------------------
ResultPtr DataBlockDeclarationStatement::execute(const ContextPtr &context) const {
  ContextPtr current(context->getDeclareLevelContext());

  IdentifierPtr identifier = this->identifier;
  const std::string &name = identifier->getName();

  ast::MacroParameterList macroParameters;
  Macro *macroObj = new Macro(name, current, macroParameters, children);
  MacroPtr macro(macroObj);
  attachDecoratable(context, macroObj);

  ContextPtr createContext(new NameHintContext(context->getInterpreter(), name, context));
  ResultPtr result = core::DataBlockClass::createInstance(createContext, macro);

  TypePtr parentDecoratorContainer = current->resolve(FMA_TYPE_DECORATORCONTAINER_MEMBER)->get();
  if (parentDecoratorContainer->isDecoratorContainer()) {
    result->get()->setMember(
      FMA_TYPE_DECORATORCONTAINER_MEMBER,
      std::dynamic_pointer_cast<DecoratorContainer>(parentDecoratorContainer)->createChild()
    );
  }

  ResultPtr member = current->getMember(name);
  member->assign(result->get());

  return result;
}

// ----------------------------------------------------------------------------
std::string DataBlockDeclarationStatement::asString(const std::string &prefix) const {
  std::ostringstream os;
  os << prefix << "datablock " << identifier->asString("") << std::endl;
  if (children) {
    os << children->asString(prefix + "  ");
  }
  os << prefix << "end" << std::endl;
  if (nextStatement) { os << nextStatement->asString(prefix); }
  return os.str();
}
