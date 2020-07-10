#include <image/ImageCollectionClass.hpp>
#include <image/ImageCollectionItemClass.hpp>
#include <image/BaseCanvasClass.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/types/Undefined.hpp>
#include <fma/types/Object.hpp>
#include <fma/core/Array.hpp>
#include <fma/core/Number.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/Project.hpp>
#include <iostream>

using namespace FMA;
using namespace FMA::types;
using namespace FMA::interpret;
using namespace FMA::image;
using namespace FMA::core;

namespace FMA {
namespace types {
  template<> const char *getInternalObjectName<ImageCollectionPtr>() { return "fmaImageCollection"; }
}
}

namespace {
  ImageCollectionPtr emptyImageCollection;
}

// ----------------------------------------------------------------------------
ClassPtr ImageCollectionClass::create(const ModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("ImageCollection", "fmaImage::ImageCollection"));
  klass->extends(ClassObject);

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", ImageCollectionClass::initialize)));
  proto->setMember("add", TypePtr(new InternalFunctionValue("add", ImageCollectionClass::add)));

  root->setMember("ImageCollection", klass);
  return klass;
}

// ----------------------------------------------------------------------------
ResultPtr ImageCollectionClass::initialize(const ContextPtr &context, const GroupedParameterList &) {
  TypePtr self = context->self();

  ImageCollectionPtr collection(new ImageCollection());
  self->setMember("__image_collection", TypePtr(new InternalImageCollectionValue(collection)));

  return Result::executed(context, self);
}

// ----------------------------------------------------------------------------
ResultPtr ImageCollectionClass::add(const ContextPtr &context, const GroupedParameterList &parameter) {
  TypePtr self = context->self();
  ImageCollectionPtr col = imageCollection(context);
  if (!col) {
    return ResultPtr(new Result());
  }
  
  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    return ResultPtr(new Result());
  }

  BaseCanvasAccessorPtr accessor = BaseCanvasClass::allocateAccessor(context->getProject(), args.front());
  if (!accessor) {
    return ResultPtr(new Result());
  }

  auto result = col->add(accessor);

  return Result::executed(context, ImageCollectionItemClass::createInstance(context, result.index, result.flipX, result.flipY));
}

// ----------------------------------------------------------------------------
const ImageCollectionPtr &ImageCollectionClass::imageCollection(const interpret::ContextPtr &context) {
  return imageCollection(context->getProject(), context->self());
}

// ----------------------------------------------------------------------------
const ImageCollectionPtr &ImageCollectionClass::imageCollection(Project *project, const types::TypePtr &value) {
  const TypePtr &obj = value->getMember("__image_collection");
  if (!obj || !obj->isInternalObjectOfType("fmaImageCollection")) {
    project->log().error() << "Unable to access image collection";
    return emptyImageCollection;
  }

  return std::dynamic_pointer_cast<InternalImageCollectionValue>(obj)->getValue();
}

// ----------------------------------------------------------------------------
