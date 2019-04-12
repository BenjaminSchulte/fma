#ifndef __FMA_INTERPRET_PROJECTCONTEXT_H__
#define __FMA_INTERPRET_PROJECTCONTEXT_H__

#include "BaseContext.hpp"

namespace FMA {
namespace interpret {

class Interpreter;

class ProjectContext : public BaseContext {
public:
  ProjectContext(Interpreter *interpreter);
  
  virtual types::TypePtr getDeclareLevelObject();
  
  virtual ResultPtr resolve(const std::string &name);
  virtual bool isValidContext() const { return true; }

  virtual ResultPtr getMember(const std::string &name);
  virtual ResultPtr setMember(const std::string &name, const types::TypePtr &type);

  virtual std::string asString() const;

protected:
};

}
}

#endif
