#ifndef __FMA_IMAGE_CANVASCLASS_H__
#define __FMA_IMAGE_CANVASCLASS_H__

#include "Canvas.hpp"
#include <fma/types/Class.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/BaseContext.hpp>

namespace FMA {
class Project;
namespace image {

typedef types::InternalObject<CanvasPtr> InternalCanvasValue;

class CanvasClass : public types::Class {
public:
  static types::ClassPtr create(const types::ModulePtr &root, const types::ClassPtr &Class);

  // STATIC
  static types::TypePtr createInstance(const interpret::ContextPtr &context);
  static interpret::ResultPtr __deserialize(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  // PROTOTYPE
  static interpret::ResultPtr initialize(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  static const CanvasPtr &canvas(const interpret::ContextPtr &context);
  static const CanvasPtr &canvas(Project *project, const types::TypePtr &type);
};

}
}

#endif
