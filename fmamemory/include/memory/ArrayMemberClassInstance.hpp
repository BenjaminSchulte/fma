#ifndef __FMA_MEMORY_ARRAYMEMBERCLASSINSTANCE_H__
#define __FMA_MEMORY_ARRAYMEMBERCLASSINSTANCE_H__

#include <fma/types/Base.hpp>

namespace FMA {
namespace memory {

class ArrayMemberClassInstance : public types::Base {
public:
  ArrayMemberClassInstance(const interpret::ContextPtr &context, const types::TypePtr &callback);

  virtual interpret::ResultPtr callWithoutDecoratorTest(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter); 
  std::string asString() const;

protected:
  interpret::ContextPtr context;
  types::TypePtr callback;
};

}
}

#endif
