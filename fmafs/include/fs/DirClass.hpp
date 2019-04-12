#ifndef __FMA_FS_DIRCLASS_H__
#define __FMA_FS_DIRCLASS_H__

#include <fma/types/Class.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/BaseContext.hpp>

namespace FMA {
namespace fs {

class DirClass : public types::Class {
public:
  static types::ClassPtr create(const types::ModulePtr &root, const types::ClassPtr &Class);

  // STATIC
  static interpret::ResultPtr mkdir(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr mkdir_p(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  // PROTOTYPE
  static interpret::ResultPtr initialize(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
};

}
}

#endif
