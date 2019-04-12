#include <image/ImageClass.hpp>
#include <image/CanvasClass.hpp>
#include <fma/core/Nil.hpp>
#include <fma/types/Undefined.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/types/Object.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/interpret/ParameterList.hpp>

using namespace FMA;
using namespace FMA::types;
using namespace FMA::interpret;
using namespace FMA::image;
using namespace FMA::core;

// ----------------------------------------------------------------------------
ClassPtr ImageClass::create(const ModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("Image", "fmaImage::Image"));
  klass->extends(ClassObject);

  klass->setMember("from_file", TypePtr(new InternalFunctionValue("from_file", ImageClass::from_file)));

  // ClassPrototypePtr proto(klass->getPrototype());
  // proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", ImageClass::initialize)));

  root->setMember("Image", klass);
  return klass;
}

// ----------------------------------------------------------------------------
TypePtr ImageClass::createInstance(const interpret::ContextPtr &context) {
  ClassPtr Image = context->getRootLevelContext()->getMember("fmaImage")->get()->getMember("Image")->asClass();
  if (!Image) {
    TypePtr result(new Undefined());
    return result;
  }

  GroupedParameterList emptyParams;
  return Image->createInstance(context, emptyParams);
}

// ----------------------------------------------------------------------------
ResultPtr ImageClass::from_file(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    return NilClass::createInstance(context);
  }

  TypePtr image(createInstance(context));

  std::string fileName = args.front()->convertToString(context);
  if (!CanvasClass::canvas(context->getProject(), image)->loadFromPng(fileName.c_str())) {
    return NilClass::createInstance(context);
  }

  return Result::executed(context, image);
}

// ----------------------------------------------------------------------------
