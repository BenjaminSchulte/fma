#ifndef __FMA_MEMORY_MEMORYDECLARATIONOBJECT_H__
#define __FMA_MEMORY_MEMORYDECLARATIONOBJECT_H__

#include <fma/types/Class.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/symbol/CalculatedNumber.hpp>

namespace FMA {
namespace interpret {
class BaseContext;
typedef std::shared_ptr<BaseContext> ContextPtr;
}
namespace memory {

class MemoryDeclarationClass : public types::Class {
public:
  static types::ClassPtr create(const types::RootModulePtr &root, const types::ClassPtr &Class);

  // STATIC

  // PROTOTYPE
  static interpret::ResultPtr initialize(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  
  static interpret::ResultPtr op_add(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr op_sub(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr op_div(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr op_mul(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr op_rem(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr op_and(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr op_or(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr op_xor(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr op_lshift(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr op_rshift(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  static interpret::ResultPtr calculation(const std::string &op, const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
};

}
}

#endif
