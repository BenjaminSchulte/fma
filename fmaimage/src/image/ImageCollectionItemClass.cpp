#include <image/ImageCollectionItemClass.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/types/Undefined.hpp>
#include <fma/types/Object.hpp>
#include <fma/core/Array.hpp>
#include <fma/core/Number.hpp>
#include <fma/core/Boolean.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/Project.hpp>
#include <iostream>

using namespace FMA;
using namespace FMA::types;
using namespace FMA::interpret;
using namespace FMA::image;
using namespace FMA::core;

// ----------------------------------------------------------------------------
ClassPtr ImageCollectionItemClass::create(const ModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("ImageCollectionItem", "fmaImage::ImageCollectionItem"));
  klass->extends(ClassObject);

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", ImageCollectionItemClass::initialize)));

  root->setMember("ImageCollectionItem", klass);
  return klass;
}

// ----------------------------------------------------------------------------
TypePtr ImageCollectionItemClass::createInstance(const interpret::ContextPtr &context, uint32_t index, bool flipX, bool flipY) {
  ClassPtr ImageCollectionItem = context->getRootLevelContext()->getMember("fmaImage")->get()->getMember("ImageCollectionItem")->asClass();
  if (!ImageCollectionItem) {
    TypePtr result(new Undefined());
    return result;
  }

  GroupedParameterList params;
  params.push_back(NumberClass::createInstance(context, index)->get());
  params.push_back(BooleanClass::createInstance(context, flipX)->get());
  params.push_back(BooleanClass::createInstance(context, flipY)->get());
  return ImageCollectionItem->createInstance(context, params);
}

// ----------------------------------------------------------------------------
ResultPtr ImageCollectionItemClass::initialize(const ContextPtr &context, const GroupedParameterList &parameter) {
  TypePtr self = context->self();
  const TypeList &args = parameter.only_args();
  if (args.size() != 3 ){
    return Result::executed(context, self);
  }

  self->setMember("index", args.at(0));
  self->setMember("flip_x", args.at(1));
  self->setMember("flip_y", args.at(2));

  return Result::executed(context, self);
}
