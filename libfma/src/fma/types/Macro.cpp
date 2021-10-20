#include <fma/ast/Statement.hpp>
#include <fma/types/Macro.hpp>
#include <fma/types/Block.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/BlockContext.hpp>
#include <fma/interpret/NestedContext.hpp>
#include <fma/interpret/InstanceContext.hpp>
#include <fma/interpret/StaticMemberContext.hpp>
#include <fma/interpret/ObjectContext.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/interpret/Interpreter.hpp>
#include <sstream>
#include <iostream>

using namespace FMA;
using namespace FMA::interpret;
using namespace FMA::ast;
using namespace FMA::types;

// ----------------------------------------------------------------------------
Macro::Macro(const std::string &name, const ContextPtr &context, const MacroParameterList &parameters, const StatementPtr &children)
  : name(name)
  , context(context)
  , children(children)
  , parameters(parameters)
  , macroSuper(NULL)
  , isDecorator(false)
{
}

// ----------------------------------------------------------------------------
void Macro::dump(const std::string &prefix) {
  std::cout << prefix << "macro " << name << std::endl;
}

// ----------------------------------------------------------------------------
ResultPtr Macro::callInDecorator(const ContextPtr &callContext, const interpret::GroupedParameterList &params, DecoratorCallType) {
  if (!children) {
    return ResultPtr(new Result());
  }
  ContextPtr runContext;
  BlockPtr block(new Block());
  const interpret::TypeList &contextArgs = params.only_context();

  ContextPtr mainContext(new BlockContext(context->getInterpreter(), block));
  TypePtr self = callContext->self();
  if (self->isObject()) {
    mainContext = ContextPtr(new NestedContext(
      context->getInterpreter(),
      mainContext,
      ContextPtr(new InstanceContext(
        context->getInterpreter(),
        self->asObject(),
        name
      ))
    ));
  } else if (self->isClass()) {
    mainContext->setMember("self", self);
    mainContext = ContextPtr(new NestedContext(
      context->getInterpreter(),
      mainContext,
      ContextPtr(new StaticMemberContext(
        context->getInterpreter(),
        self->asClass(),
        name
      ))
    ));
  } else if (callContext->hasCallee() && callContext->getCallee()->isClass()) {
    mainContext->setMember("self", callContext->getCallee());
    mainContext = ContextPtr(new NestedContext(
      context->getInterpreter(),
      mainContext,
      ContextPtr(new StaticMemberContext(
        context->getInterpreter(),
        callContext->getCallee()->asClass(),
        name
      ))
    ));
  }

  if (!contextArgs.size()) {
    runContext = ContextPtr(new NestedContext(
      context->getInterpreter(),
      mainContext,
      context
    ));
  } else {
    ContextPtr innerBlock;
    if (contextArgs.front()->isObject()) {
      innerBlock = ContextPtr(new InstanceContext(
        context->getInterpreter(),
        contextArgs.front()->asObject(),
        ""
      ));
    } else {
      innerBlock = ContextPtr(new BlockContext(
        context->getInterpreter(),
        contextArgs.front()
      ));
    }

    runContext = ContextPtr(new NestedContext(
      context->getInterpreter(),
      ContextPtr(new NestedContext(
        context->getInterpreter(),
        mainContext,
        innerBlock
      )),
      context
    ));
  }

  if (callContext->hasCallee()) {
    block->setMember("callee", callContext->getCallee());
  }

  auto _stack = callContext->getInterpreter()->stack().enter(children->getLocation());
  block->setMembersFromParameter(callContext, runContext, parameters, params);
  LabelStatement::registerLabels(runContext, runContext->getNameHint(), children);

  if (macroSuper) {
    block->setMember("//super", macroSuper);
  }
  
  ResultPtr result = runContext->execute(children);
  result->noEarlyReturn();
  return result;
}

// ----------------------------------------------------------------------------
std::string Macro::asString() const {
  std::ostringstream os;
  os << "<Macro " << name << ">";
  return os.str();
}

// ----------------------------------------------------------------------------

