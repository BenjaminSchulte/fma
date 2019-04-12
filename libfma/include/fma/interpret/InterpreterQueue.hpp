#ifndef __FMA_INTERPRET_INTERPRETERQUEUE_H__
#define __FMA_INTERPRET_INTERPRETERQUEUE_H__

#include <memory>
#include <list>
#include "ParameterList.hpp"
#include "BaseContext.hpp"
#include "../types/Macro.hpp"

namespace FMA {
namespace interpret {
class InterpreterQueueItem {
public:
  InterpreterQueueItem() {}
  virtual ~InterpreterQueueItem() {}

  virtual bool execute()=0;
};

class MacroInterpreterQueueItem : public InterpreterQueueItem {
public:
  MacroInterpreterQueueItem(const interpret::ContextPtr &context, const types::MacroPtr &macro, const interpret::GroupedParameterList &parameter, bool useAsGlobalContext)
    : context(context)
    , macro(macro)
    , parameter(parameter)
    , useAsGlobalContext(useAsGlobalContext)
  {}

  bool execute();
    
protected:
  interpret::ContextPtr context;
  types::MacroPtr macro;
  interpret::GroupedParameterList parameter;
  bool useAsGlobalContext;
};

class InterpreterQueue {
public:
  InterpreterQueue(class Interpreter *interpreter);
  virtual ~InterpreterQueue();

  void queue(const interpret::ContextPtr &context, const types::MacroPtr &macro, const interpret::GroupedParameterList &parameter, bool useAsGlobalContext=false);
  bool execute();
  bool more() const;

protected:
  void removeAllItems();

  class Interpreter *interpreter;
  std::list<InterpreterQueueItem*> items;
};

}
}

#endif