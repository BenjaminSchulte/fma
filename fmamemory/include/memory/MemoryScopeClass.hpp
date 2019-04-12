#ifndef __FMA_MEMORY_MEMORYSCOPEOBJECT_H__
#define __FMA_MEMORY_MEMORYSCOPEOBJECT_H__

#include <fma/types/Class.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/interpret/Result.hpp>
#include <memory/MemoryScope.hpp>

namespace FMA {
class Project;
namespace interpret {
class BaseContext;
typedef std::shared_ptr<BaseContext> ContextPtr;
}
namespace memory {

class MemoryScopeClass : public types::Class {
public:
  static types::ClassPtr create(const types::RootModulePtr &root, const types::ClassPtr &Class);

  // STATIC

  // PROTOTYPE
  static interpret::ResultPtr initialize(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr include(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr allow(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr deny(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr to_sym(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);


  static const MemoryScopePtr &asMemoryScope(const interpret::ContextPtr &context);
  static const MemoryScopePtr &asMemoryScope(Project *project, const types::TypePtr &);
};

}
}

#endif
