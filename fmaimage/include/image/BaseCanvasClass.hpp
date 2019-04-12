#ifndef __FMA_IMAGE_BASECANVASCLASS_H__
#define __FMA_IMAGE_BASECANVASCLASS_H__

#include "CanvasAccessor.hpp"
#include <fma/types/Class.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/BaseContext.hpp>

namespace FMA {
class Project;
namespace image {

typedef std::shared_ptr<BaseCanvasAccessor> BaseCanvasAccessorPtr;

class BaseCanvasClass : public types::Class {
public:
  static types::ClassPtr create(const types::ModulePtr &root, const types::ClassPtr &Class);

  // STATIC

  // PROTOTYPE
  static interpret::ResultPtr width(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr height(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr rows(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr cols(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr pixels(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr chunk(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr required_palette(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr apply_palette(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr dump(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr __serialize(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  static BaseCanvasAccessorPtr allocateAccessor(const interpret::ContextPtr &context);
  static BaseCanvasAccessorPtr allocateAccessor(Project *project, const types::TypePtr &value);
};

}
}

#endif
