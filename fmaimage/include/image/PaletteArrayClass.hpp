#ifndef __FMA_IMAGE_PALETTEARRAYCLASS_H__
#define __FMA_IMAGE_PALETTEARRAYCLASS_H__

#include <fma/types/Class.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/BaseContext.hpp>

namespace FMA {
class Project;
namespace image {

class PaletteArrayClass : public types::Class {
public:
  static types::ClassPtr create(const types::ModulePtr &root, const types::ClassPtr &Class);

  // STATIC
  static interpret::ResultPtr createInstance(const interpret::ContextPtr &context, std::vector<types::TypePtr> &);

  // INSTANCE
  static interpret::ResultPtr index_of(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr find_matching_palettes(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
};

}
}

#endif
