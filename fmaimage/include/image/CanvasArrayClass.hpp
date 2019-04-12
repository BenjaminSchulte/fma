#ifndef __FMA_IMAGE_CANVASARRAYCLASS_H__
#define __FMA_IMAGE_CANVASARRAYCLASS_H__

#include <fma/types/Class.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/BaseContext.hpp>

namespace FMA {
class Project;
namespace image {

class CanvasArrayClass : public types::Class {
public:
  static types::ClassPtr create(const types::ModulePtr &root, const types::ClassPtr &Class);

  // STATIC
  static interpret::ResultPtr createInstance(const interpret::ContextPtr &context, std::vector<types::TypePtr> &);
};

}
}

#endif
