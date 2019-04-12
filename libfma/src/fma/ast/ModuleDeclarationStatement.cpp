#include <sstream>
#include <fma/ast/Expression.hpp>
#include <fma/ast/Statement.hpp>
#include <fma/ast/Identifier.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/NestedContext.hpp>
#include <fma/interpret/ModuleContext.hpp>
#include <fma/types/Module.hpp>
#include <fma/types/Undefined.hpp>
#include <fma/types/DecoratorContainer.hpp>
#include <fma/Project.hpp>

using namespace FMA::ast;
using namespace FMA::interpret;
using namespace FMA::types;

// ----------------------------------------------------------------------------
ModuleDeclarationStatement::ModuleDeclarationStatement(Identifier *identifier, Statement *children)
  : identifier(identifier)
  , children(children)
{
}

// ----------------------------------------------------------------------------
ModuleDeclarationStatement::~ModuleDeclarationStatement() {
}

// ----------------------------------------------------------------------------
ResultPtr ModuleDeclarationStatement::execute(const ContextPtr &context) const {
  ContextPtr current(identifier->isRoot() ? context->getRootLevelContext() : context->getDeclareLevelContext());
  IdentifierPtr identifier = this->identifier;
  ResultPtr result = Result::executed(context, TypePtr(new Undefined()));

  while (identifier) {
    const std::string &name = identifier->getName();
    ResultPtr member = current->getMember(name);
    ModulePtr module;
    if (!member->isUndefined()) {
      if (!member->isModule()) {
        context->log().error() << location << "The context already has a member '" << name << "' which is no module";
        return ResultPtr(new Result());
      }

      TypePtr memberValue = member->get();
      module = std::dynamic_pointer_cast<Module>(memberValue);
    } else {
      module = ModulePtr(new Module(name));
      member->assign(module);

      TypePtr parentDecoratorContainer = current->resolve(FMA_TYPE_DECORATORCONTAINER_MEMBER)->get();
      if (parentDecoratorContainer->isDecoratorContainer()) {
        module->setMember(
          FMA_TYPE_DECORATORCONTAINER_MEMBER,
          std::dynamic_pointer_cast<DecoratorContainer>(parentDecoratorContainer)->createChild()
        );
      }
    }

    current = ContextPtr(new NestedContext(
      current->getInterpreter(),
      ContextPtr(new ModuleContext(
        current->getInterpreter(),
        module
      )),
      current
    ));

    identifier = identifier->getNextIdentifier();
  }

  if (children) {
    result = current->execute(children);
  }

  return result;
}

// ----------------------------------------------------------------------------
std::string ModuleDeclarationStatement::asString(const std::string &prefix) const {
  std::ostringstream os;
  os << prefix << "module " << identifier->asString("") << std::endl;
  if (children) {
    os << children->asString(prefix + "  ");
  }
  os << prefix << "end" << std::endl;
  if (nextStatement) { os << nextStatement->asString(prefix); }
  return os.str();
}
