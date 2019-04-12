#ifndef __FMA_MEMORY_MEMORYVARIABLEPOINTEROBJECT_H__
#define __FMA_MEMORY_MEMORYVARIABLEPOINTEROBJECT_H__

#include <fma/types/Class.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/interpret/Result.hpp>
#include <memory/MemoryAllocation.hpp>

namespace FMA {
namespace interpret {
class BaseContext;
typedef std::shared_ptr<BaseContext> ContextPtr;
}
namespace memory {

class MemoryVariablePointerClass : public types::Class {
public:
  static types::ClassPtr create(const types::RootModulePtr &root, const types::ClassPtr &Class);

  // STATIC
  static interpret::ResultPtr createInstance(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  // PROTOTYPE
  static interpret::ResultPtr initialize(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr to_sym(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr op_index(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
};

}
}

#endif
