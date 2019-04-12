#ifndef __FMA_TYPES_MACRO_H__
#define __FMA_TYPES_MACRO_H__

#include "Decoratable.hpp"
#include <vector>

namespace FMA {
namespace ast {
  typedef std::shared_ptr<class MacroParameter> MacroParameterPtr;
  typedef std::shared_ptr<class Statement> StatementPtr;
  typedef std::vector<MacroParameterPtr> MacroParameterList;
}
namespace interpret {
  typedef std::shared_ptr<class BaseContext> ContextPtr;
  struct GroupedParameterList;
}
namespace types {

class Macro : public Decoratable {
public:
  Macro(const std::string &name, const interpret::ContextPtr &context, const ast::MacroParameterList &parameters, const ast::StatementPtr &children);
  
  virtual bool isMacro() const { return true; }
  virtual MacroPtr asMacro() { return std::dynamic_pointer_cast<Macro>(getPointer()); }

  virtual interpret::ResultPtr callInDecorator(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameters, DecoratorCallType type);

  virtual std::string asString() const;
  virtual void dump(const std::string &prefix);
  
  inline void setIsDecorator() { isDecorator = true; }

protected:
  std::string name;
  interpret::ContextPtr context;
  ast::StatementPtr children;
  ast::MacroParameterList parameters;
  bool isDecorator;
};

}
}

#endif
