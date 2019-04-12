#ifndef __FMA_INTERPRET_PROTOTYPECONTEXT_H__
#define __FMA_INTERPRET_PROTOTYPECONTEXT_H__

#include "BaseContext.hpp"

namespace FMA {
namespace types {
class Class;
typedef std::shared_ptr<Class> ClassPtr;
}
namespace interpret {

class Interpreter;

class PrototypeContext : public BaseContext {
public:
  PrototypeContext(Interpreter *interpreter, const types::ClassPtr &klass);

  virtual types::TypePtr getDeclareLevelObject();
  virtual ContextPtr getDeclareLevelContext();
  virtual ContextPtr getMacroDeclareLevelContext();
  virtual bool isValidContext() const { return true; }
  
  virtual ResultPtr resolve(const std::string &name);

  virtual ResultPtr getMember(const std::string &name);
  virtual ResultPtr setMember(const std::string &name, const types::TypePtr &type);
  //virtual ContextPtr withoutInstances() { return ContextPtr(new BaseContext(interpreter)); };

  virtual std::string asString() const;

protected:
  types::ClassPtr klass;
};

}
}

#endif
