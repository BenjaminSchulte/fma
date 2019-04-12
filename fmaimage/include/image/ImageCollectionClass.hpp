#ifndef __FMA_IMAGE_IMAGECOLLECTIONCLASS_H__
#define __FMA_IMAGE_IMAGECOLLECTIONCLASS_H__

#include "./ImageCollection.hpp"
#include <fma/types/Class.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/BaseContext.hpp>

namespace FMA {
class Project;
namespace image {

typedef types::InternalObject<ImageCollectionPtr> InternalImageCollectionValue;

class ImageCollectionClass : public types::Class {
public:
  static types::ClassPtr create(const types::ModulePtr &root, const types::ClassPtr &Class);

  // STATIC

  // PROTOTYPE
  static interpret::ResultPtr initialize(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr add(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  static const ImageCollectionPtr &imageCollection(const interpret::ContextPtr &context);
  static const ImageCollectionPtr &imageCollection(Project *project, const types::TypePtr &type);
};

}
}

#endif
