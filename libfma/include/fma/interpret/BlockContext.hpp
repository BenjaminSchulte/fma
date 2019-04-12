#ifndef __FMA_INTERPRET_BLOCKCONTEXT_H__
#define __FMA_INTERPRET_BLOCKCONTEXT_H__

#include "BaseContext.hpp"

namespace FMA {
namespace types {
  class Base;
  typedef std::shared_ptr<Base> TypePtr;
}
namespace interpret {

class Interpreter;

class BlockContext : public BaseContext {
public:
  BlockContext(Interpreter *interpreter, const types::TypePtr &object);
  
  virtual types::TypePtr getDeclareLevelObject();
  virtual bool isValidContext() const { return true; }

  virtual ResultPtr getMember(const std::string &name);
  virtual ResultPtr setMember(const std::string &name, const types::TypePtr &type);

  virtual std::string asString() const;

protected:
  types::TypePtr object;
};

}
}

#endif
