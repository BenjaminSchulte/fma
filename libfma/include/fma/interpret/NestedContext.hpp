#ifndef __FMA_INTERPRET_NESTEDCONTEXT_H__
#define __FMA_INTERPRET_NESTEDCONTEXT_H__

#include "BaseContext.hpp"

namespace FMA {
namespace interpret {

class Interpreter;

class NestedContext : public BaseContext {
public:
  NestedContext(Interpreter *interpreter, const ContextPtr &current, const ContextPtr &parent);
  
  virtual types::TypePtr getParentFunction();
  virtual types::TypePtr getDeclareLevelObject();
  virtual ContextPtr getDeclareLevelContext();
  virtual ContextPtr getMacroDeclareLevelContext();
  virtual ContextPtr withoutInstances();
  
  virtual ResultPtr resolve(const std::string &name);
  virtual bool isValidContext() const { return true; }

  virtual ResultPtr getMember(const std::string &name);
  virtual ResultPtr setMember(const std::string &name, const types::TypePtr &type);

  virtual std::string asString() const;
  virtual std::string getNameHint() const;

protected:
  ContextPtr current;
  ContextPtr parent;
};

}
}

#endif
