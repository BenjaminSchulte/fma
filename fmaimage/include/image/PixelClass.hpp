#ifndef __FMA_IMAGE_PIXELCLASS_H__
#define __FMA_IMAGE_PIXELCLASS_H__

#include <fma/types/Class.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/BaseContext.hpp>

namespace FMA {
namespace image {

class PixelClass : public types::Class {
public:
  static types::ClassPtr create(const types::ModulePtr &root, const types::ClassPtr &Class);

  // STATIC
  static types::TypePtr createInstance(const interpret::ContextPtr &context, const types::TypePtr &parent, uint32_t left, uint32_t top);

  // PROTOTYPE
  static interpret::ResultPtr initialize(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr to_s(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
};

}
}

#endif
