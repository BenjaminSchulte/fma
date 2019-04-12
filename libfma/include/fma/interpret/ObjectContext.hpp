#ifndef __FMA_INTERPRET_OBJECTCONTEXT_H__
#define __FMA_INTERPRET_OBJECTCONTEXT_H__

#include "BaseContext.hpp"

namespace FMA {
namespace types {
  class Base;
  typedef std::shared_ptr<Base> TypePtr;
}
namespace interpret {

class Interpreter;

class ObjectContext : public BaseContext {
public:
  ObjectContext(Interpreter *interpreter, const types::TypePtr &object);
  
  virtual types::TypePtr self();
  virtual types::TypePtr getDeclareLevelObject();
  virtual bool isValidContext() const { return true; }

  virtual ResultPtr getMember(const std::string &name);
  virtual ResultPtr setMember(const std::string &name, const types::TypePtr &type);
  virtual ResultPtr resolve(const std::string &name);

  virtual std::string asString() const;

protected:
  types::TypePtr object;
};

}
}

#endif
