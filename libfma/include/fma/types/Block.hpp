#ifndef __FMA_TYPES_BLOCK_H__
#define __FMA_TYPES_BLOCK_H__

#include "Base.hpp"

namespace FMA {
namespace ast {
  typedef std::shared_ptr<MacroParameter> MacroParameterPtr;
  typedef std::vector<MacroParameterPtr> MacroParameterList;
}
namespace types {

class Block : public Base {
public:
  Block();

  virtual bool isBlock() const { return true; }

  virtual std::string asString() const;
  virtual interpret::ResultPtr callWithoutDecoratorTest(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter); 

  void setMembersFromParameter(const interpret::ContextPtr &callContext, const interpret::ContextPtr &blockContext, const ast::MacroParameterList &expected, const interpret::GroupedParameterList &given); 
protected:
};

}
}

#endif
