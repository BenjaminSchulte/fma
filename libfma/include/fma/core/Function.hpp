#ifndef __FMA_CORE_FUNCTION_H__
#define __FMA_CORE_FUNCTION_H__

#include <fma/types/Class.hpp>
#include <fma/types/RootModule.hpp>

namespace FMA {
namespace core {

class FunctionClass : public types::Class {
public:
  static types::ClassPtr create(const types::RootModulePtr &root, const types::ClassPtr &Class);
  static interpret::ResultPtr createInstance(const interpret::ContextPtr &context, const types::MacroPtr &macro);

  // PROTOTYPE
  static interpret::ResultPtr initialize(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
};

}
}

#endif
