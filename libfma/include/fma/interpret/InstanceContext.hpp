#ifndef __FMA_INTERPRET_INSTANCECONTEXT_H__
#define __FMA_INTERPRET_INSTANCECONTEXT_H__

#include "BaseContext.hpp"

namespace FMA {
namespace types {
  class Base;
  typedef std::shared_ptr<Base> TypePtr;
}
namespace interpret {

class Interpreter;

class InstanceContext : public BaseContext {
public:
  InstanceContext(Interpreter *interpreter, const types::ObjectPtr &object, const std::string &name);
  
  virtual types::TypePtr self();
  virtual types::TypePtr getDeclareLevelObject();
  virtual types::TypePtr getParentFunction();
  virtual bool isValidContext() const { return true; }

  virtual ResultPtr getMember(const std::string &name);
  virtual ResultPtr setMember(const std::string &name, const types::TypePtr &type);
  virtual ResultPtr resolve(const std::string &name);
  virtual ContextPtr withoutInstances() { return ContextPtr(new BaseContext(interpreter)); };
  virtual std::string asString() const;

protected:
  std::string name;
  types::ObjectPtr object;
};

}
}

#endif
