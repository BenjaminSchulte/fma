#ifndef __FMA_CORE_IMMEDIATENUMBER_H__
#define __FMA_CORE_IMMEDIATENUMBER_H__

#include <fma/types/Class.hpp>
#include <fma/types/RootModule.hpp>

namespace FMA {
namespace core {

class ImmediateNumberClass : public types::Class {
public:
  static types::ClassPtr create(const types::RootModulePtr &root, const types::ClassPtr &Class);
  static interpret::ResultPtr createInstance(const interpret::ContextPtr &context, const types::TypePtr &value);

  // PROTOTYPE
  static interpret::ResultPtr initialize(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
};

}
}

#endif
