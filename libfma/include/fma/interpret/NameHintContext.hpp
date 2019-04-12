#ifndef __FMA_INTERPRET_NAMEHINTCONTEXT_H__
#define __FMA_INTERPRET_NAMEHINTCONTEXT_H__

#include "BaseContext.hpp"

namespace FMA {
namespace interpret {

class Interpreter;

class NameHintContext : public BaseContext {
public:
  NameHintContext(Interpreter *interpreter, const std::string &name, const ContextPtr &parent);
  
  virtual types::TypePtr getDeclareLevelObject();
  
  virtual ResultPtr resolve(const std::string &name);
  virtual bool isValidContext() const { return true; }

  virtual ResultPtr getMember(const std::string &name);
  virtual ResultPtr setMember(const std::string &name, const types::TypePtr &type);

  virtual std::string asString() const;
  virtual std::string getNameHint() const;

protected:
  std::string name;
  ContextPtr parent;
};

}
}

#endif
