#ifndef __FMA_INTERPRET_BASECONTEXT_H__
#define __FMA_INTERPRET_BASECONTEXT_H__

#include <memory>
#include "LogWithStack.hpp"

namespace FMA {
class Project;
namespace ast {
class Node;
typedef std::shared_ptr<Node> NodePtr;
}
namespace types {
class Base;
typedef std::shared_ptr<Base> TypePtr;
}
namespace interpret {
  
class Interpreter;
class Result;
typedef std::shared_ptr<Result> ResultPtr;
typedef std::shared_ptr<class BaseContext> ContextPtr;

class BaseContext : public std::enable_shared_from_this<BaseContext> {
public:
  BaseContext(Interpreter *interpreter);
  virtual ~BaseContext() {}

  /**
   * Returns the current context on statement execute level
   */
  virtual types::TypePtr getParentFunction();
  virtual ContextPtr getStatementLevelContext();
  virtual ContextPtr getRootLevelContext();
  virtual ContextPtr getDeclareLevelContext();
  virtual ContextPtr getMacroDeclareLevelContext() { return getDeclareLevelContext(); }
  virtual ContextPtr withoutInstances() { return getPointer(); }
  virtual bool isValidContext() const { return false; }
  virtual types::TypePtr self();
  virtual types::TypePtr getDeclareLevelObject();

  virtual bool hasCallee() const { return !!callee; }
  virtual types::TypePtr getCallee() const { return callee; };
  virtual void setCallee(const types::TypePtr &newCallee) { callee = newCallee; }

  virtual std::string asString() const;

  virtual ResultPtr execute(const ast::NodePtr &node);
  virtual ResultPtr resolve(const std::string &name);

  virtual ResultPtr getMember(const std::string &name);
  virtual ResultPtr setMember(const std::string &name, const types::TypePtr &type);

  inline Interpreter *getInterpreter() const { return interpreter; }
  Project *getProject() const;
  LogWithStack log() { return LogWithStack(interpreter); }

  inline std::shared_ptr<BaseContext> getPointer() {
    return shared_from_this();
  }

  virtual std::string getNameHint() const;

protected:
  Interpreter *interpreter;
  types::TypePtr callee;
};

}
}

#endif
