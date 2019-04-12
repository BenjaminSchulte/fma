#ifndef __FMA_IMAGE_CANVASVIEWCLASS_H__
#define __FMA_IMAGE_CANVASVIEWCLASS_H__

#include "CanvasView.hpp"
#include <fma/types/Class.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/BaseContext.hpp>

namespace FMA {
namespace image {

typedef types::InternalObject<CanvasViewPtr> InternalCanvasViewValue;

class CanvasViewClass : public types::Class {
public:
  static types::ClassPtr create(const types::ModulePtr &root, const types::ClassPtr &Class);

  // STATIC
  static types::TypePtr createInstance(const interpret::ContextPtr &context, const types::TypePtr &parent, uint32_t left, uint32_t top, uint32_t width, uint32_t height);

  // PROTOTYPE
  static interpret::ResultPtr initialize(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  static const CanvasViewPtr &canvasView(const interpret::ContextPtr &context);
  static const CanvasViewPtr &canvasView(Project *project, const types::TypePtr &type);
};

}
}

#endif
