#ifndef __FMA_INTERPRET_MODULECONTEXT_H__
#define __FMA_INTERPRET_MODULECONTEXT_H__

#include "BaseContext.hpp"

namespace FMA {
namespace types {
class Module;
typedef std::shared_ptr<Module> ModulePtr;
}
namespace interpret {

class Interpreter;

class ModuleContext : public BaseContext {
public:
  ModuleContext(Interpreter *interpreter, const types::ModulePtr &module);

  virtual types::TypePtr getDeclareLevelObject();
  
  virtual ResultPtr resolve(const std::string &name);
  virtual bool isValidContext() const { return true; }

  virtual ResultPtr getMember(const std::string &name);
  virtual ResultPtr setMember(const std::string &name, const types::TypePtr &type);

  virtual std::string asString() const;
  virtual std::string getNameHint() const;

protected:
  types::ModulePtr module;
};

}
}

#endif
