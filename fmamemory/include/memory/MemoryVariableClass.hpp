#ifndef __FMA_MEMORY_MEMORYVARIABLEOBJECT_H__
#define __FMA_MEMORY_MEMORYVARIABLEOBJECT_H__

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

class MemoryVariableClass : public types::Class {
public:
  static types::ClassPtr create(const types::RootModulePtr &root, const types::ClassPtr &Class);

  // STATIC

  // PROTOTYPE
  static interpret::ResultPtr initialize(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr to_sym(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr op_index(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  
  static interpret::ResultPtr op_index_array(const MemoryAllocationPtr &memory, const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr op_index_member(const MemoryAllocationPtr &memory, const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  static const MemoryAllocationPtr &asMemoryAllocation(const interpret::ContextPtr &context);
  static const MemoryAllocationPtr &asMemoryAllocation(Project *project, const types::TypePtr &);

  static uint8_t getInternalTypeSize(const std::string &);
};

}
}

#endif
