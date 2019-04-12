#ifndef __FMA_IMAGE_IMAGECOLLECTIONITEMCLASS_H__
#define __FMA_IMAGE_IMAGECOLLECTIONITEMCLASS_H__

#include <fma/types/Class.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/BaseContext.hpp>

namespace FMA {
class Project;
namespace image {

class ImageCollectionItemClass : public types::Class {
public:
  static types::ClassPtr create(const types::ModulePtr &root, const types::ClassPtr &Class);

  // STATIC
  static types::TypePtr createInstance(const interpret::ContextPtr &context, uint32_t index, bool flipX, bool flipY);

  // PROTOTYPE
  static interpret::ResultPtr initialize(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
};

}
}

#endif
