#include <fma/interpret/InterpreterQueue.hpp>
#include <fma/interpret/Interpreter.hpp>
#include <fma/Project.hpp>
#include <iostream>

using namespace FMA;
using namespace FMA::interpret;
using namespace FMA::types;

// ----------------------------------------------------------------------------
bool MacroInterpreterQueueItem::execute() {
  context->getProject()->log().trace() << "Executing queued macro " << macro->asString();

  if (useAsGlobalContext) {
    context->getInterpreter()->setGlobalContext(context);
  }

  macro->call(context, parameter);

  if (useAsGlobalContext) {
    context->getInterpreter()->setGlobalContext(NULL);
  }

  return true;
}


// ----------------------------------------------------------------------------
InterpreterQueue::InterpreterQueue(Interpreter *interpreter)
  : interpreter(interpreter)
{
}

// ----------------------------------------------------------------------------
InterpreterQueue::~InterpreterQueue() {
  removeAllItems();
}

// ----------------------------------------------------------------------------
void InterpreterQueue::removeAllItems() {
  while (items.size()) {
    InterpreterQueueItem *item = items.back();
    items.pop_back();
    delete item;
  }
}

// ----------------------------------------------------------------------------
bool InterpreterQueue::execute() {
  InterpreterQueueItem *item = items.front();
  items.pop_front();

  bool result = item->execute();

  delete item;
  return result;
}

// ----------------------------------------------------------------------------
bool InterpreterQueue::more() const {
  return !!items.size();
}

// ----------------------------------------------------------------------------
void InterpreterQueue::queue(const ContextPtr &context, const MacroPtr &macro, const GroupedParameterList &parameter, bool useAsGlobalContext) {
  items.push_back(new MacroInterpreterQueueItem(context, macro, parameter, useAsGlobalContext));
}

// ----------------------------------------------------------------------------
