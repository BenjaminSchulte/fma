#ifndef __FMA_INTERPRET_INTERPRETER_H__
#define __FMA_INTERPRET_INTERPRETER_H__

#include <memory>
#include "CallStack.hpp"

namespace FMA {
class Project;
namespace interpret {
typedef std::shared_ptr<class BaseContext> ContextPtr;
}
namespace ast {
class Node;
typedef std::shared_ptr<Node> NodePtr;
}

namespace interpret {
class Result;
typedef std::shared_ptr<Result> ResultPtr;

class Interpreter {
public:
  Interpreter(Project *project);
  virtual ~Interpreter();

  ResultPtr execute(const ast::NodePtr &node);
  ResultPtr executeWithoutPostProcess(const ast::NodePtr &node);
  void postProcess();

  inline Project *getProject() const { return project; }
  inline class InterpreterQueue *getQueue() const { return queue; }
  inline void setGlobalContext(const ContextPtr &context) { globalContext = context; }
  inline const ContextPtr &getGlobalContext() const { return globalContext; }
  inline CallStack &stack() { return _stack; }

protected:
  Project *project;
  class InterpreterQueue *queue;
  ContextPtr globalContext;
  CallStack _stack;
};

}
}

#endif