#include <sstream>
#include <fma/ast/Expression.hpp>
#include <fma/ast/Statement.hpp>
#include <fma/ast/Identifier.hpp>
#include <fma/types/Macro.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/types/Class.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/BaseContext.hpp>

using namespace FMA::ast;
using namespace FMA::interpret;
using namespace FMA::types;

// ----------------------------------------------------------------------------
MacroDeclarationStatement::MacroDeclarationStatement(Identifier *identifier, const MacroParameterList &parameters, Statement *children, bool isStatic)
  : identifier(identifier)
  , children(children)
  , parameters(parameters)
  , isStatic(isStatic)
  , isDecorator(false)
{
}

// ----------------------------------------------------------------------------
ResultPtr MacroDeclarationStatement::execute(const ContextPtr &context) const {
  ContextPtr current(isStatic ? context->getDeclareLevelContext() : context->getMacroDeclareLevelContext());

  IdentifierPtr identifier = this->identifier;
  const std::string &name = identifier->getName();
  ResultPtr member = current->getMember(name);

  Macro *macroObj = new Macro(name, current, parameters, children);
  MacroPtr macro(macroObj);
  member->assign(macro);

  TypePtr target = current->getDeclareLevelObject();
  if (target->isClassPrototype()) {
    ClassPrototypePtr klassPtr = target->asClassPrototype();
    ClassPtr klass = klassPtr->getClass();

    if (klass) {
      for (const ClassPtr &parent : klass->getParents()) {
        if (!parent->hasPrototypeMember(name)) {
          continue;
        }

        TypePtr superCall = parent->getPrototypeMember(name);
        if (superCall->isMacro()) {
          macro->setSuper(superCall->asMacro());
        }
        break;
      }
    }
  } else if (target->isClass()) {
    for (const ClassPtr &parent : target->asClass()->getParents()) {
      if (!parent->hasMember(name)) {
        continue;
      }

      TypePtr superCall = parent->getMember(name);
      if (superCall->isMacro()) {
        macro->setSuper(superCall->asMacro());
      }

      break;
    }
  }

  if (isDecorator) {
    macro->setIsDecorator();
  }

  attachDecoratable(context, macroObj);

  return ResultPtr(new Result(context, macro));
}

// ----------------------------------------------------------------------------
std::string MacroDeclarationStatement::asString(const std::string &prefix) const {
  std::ostringstream os;
  os << prefix << "macro " << identifier->asString("") << std::endl;
  if (children) {
    os << children->asString(prefix + "  ");
  }
  os << prefix << "end" << std::endl;
  if (nextStatement) { os << nextStatement->asString(prefix); }
  return os.str();
}
