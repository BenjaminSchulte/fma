#ifndef __FMA_IMAGE_PALETTECLASS_H__
#define __FMA_IMAGE_PALETTECLASS_H__

#include "Palette.hpp"
#include <fma/types/Class.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/BaseContext.hpp>

namespace FMA {
class Project;
namespace image {

typedef types::InternalObject<PalettePtr> InternalPaletteValue;

class PaletteClass : public types::Class {
public:
  static types::ClassPtr create(const types::ModulePtr &root, const types::ClassPtr &Class);

  // STATIC
  static types::TypePtr createInstance(const interpret::ContextPtr &context);
  static interpret::ResultPtr __deserialize(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  // PROTOTYPE
  static interpret::ResultPtr initialize(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr add(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr colors(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr num_colors(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr chunk(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr dump(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr __serialize(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  static const PalettePtr &palette(const interpret::ContextPtr &context);
  static const PalettePtr &palette(Project *project, const types::TypePtr &type);
};

}
}

#endif
