#ifndef __FMA_CORE_RANGE_H__
#define __FMA_CORE_RANGE_H__

#include <fma/types/Class.hpp>
#include <fma/types/RootModule.hpp>

namespace FMA {
namespace core {

class RangeClass : public types::Class {
public:
  static types::ClassPtr create(const types::RootModulePtr &root, const types::ClassPtr &Class);
  static interpret::ResultPtr createInstance(const interpret::ContextPtr &context, const int64_t &first, const int64_t &last);

  // PROTOTYPE
  static interpret::ResultPtr first(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr initialize(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr last(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr to_s(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
};

}
}

#endif
