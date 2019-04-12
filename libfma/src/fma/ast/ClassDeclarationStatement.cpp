#include <sstream>
#include <fma/ast/Expression.hpp>
#include <fma/ast/Statement.hpp>
#include <fma/ast/Identifier.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/NestedContext.hpp>
#include <fma/interpret/ClassContext.hpp>
#include <fma/serialize/SerializerRegistry.hpp>
#include <fma/types/Module.hpp>
#include <fma/types/Class.hpp>
#include <fma/types/Undefined.hpp>
#include <fma/Project.hpp>

using namespace FMA::ast;
using namespace FMA::interpret;
using namespace FMA::types;

// ----------------------------------------------------------------------------
ClassDeclarationStatement::ClassDeclarationStatement(Identifier *identifier, Identifier *parent, Statement *children)
  : identifier(identifier)
  , parent(parent)
  , children(children)
{
}

// ----------------------------------------------------------------------------
ClassDeclarationStatement::~ClassDeclarationStatement() {
}

// ----------------------------------------------------------------------------
ResultPtr ClassDeclarationStatement::execute(const ContextPtr &context) const {
  ContextPtr current(context->getDeclareLevelContext());
  ResultPtr result = Result::executed(context, TypePtr(new Undefined()));

  IdentifierPtr identifier = this->identifier;
  const std::string &name = identifier->getName();
  ResultPtr member = current->getMember(name);
  ClassPtr klass;

  const std::string fullName = context->getNameHint() + "::" + name;
  if (member->isUndefined()) {
    klass = ClassPtr(new Class(name, fullName));
    context->getProject()->serializer()->registerClass(fullName, klass);
    
    ClassPtr parentKlass;
    if (parent) {
      parentKlass = context->execute(parent)->get()->asClass();
    }

    if (!parentKlass) {
      parentKlass = context->getRootLevelContext()->getMember("Class")->asClass();
    }

    klass->extends(parentKlass);

    member->assign(klass);
  } else if (member->isClass()) {
    klass = member->asClass();
  } else {
    context->log().error() << "Trying to access class " << name << " which is a " << member->asString();
    return member;
  }

  if (children) {
    ContextPtr childContext(new NestedContext(
      current->getInterpreter(),
      ContextPtr(new ClassContext(
        current->getInterpreter(),
        klass
      )),
      current
    ));

    result = childContext->execute(children);
  }

  return result;
}

// ----------------------------------------------------------------------------
std::string ClassDeclarationStatement::asString(const std::string &prefix) const {
  std::ostringstream os;
  os << prefix << "class " << identifier->asString("") << std::endl;
  if (children) {
    os << children->asString(prefix + "  ");
  }
  os << prefix << "end" << std::endl;
  if (nextStatement) { os << nextStatement->asString(prefix); }
  return os.str();
}
