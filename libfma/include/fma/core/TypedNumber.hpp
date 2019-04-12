#ifndef __FMA_CORE_TYPEDNUMBER_H__
#define __FMA_CORE_TYPEDNUMBER_H__

#include <fma/types/Class.hpp>
#include <fma/types/RootModule.hpp>

namespace FMA {
namespace core {

class TypedNumberClass : public types::Class {
public:
  static types::ClassPtr create(const types::RootModulePtr &root, const types::ClassPtr &Class);
  static interpret::ResultPtr createInstance(const interpret::ContextPtr &context, const types::TypePtr &value, const std::string &name);

  // PROTOTYPE
  static interpret::ResultPtr initialize(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
};

}
}

#endif
