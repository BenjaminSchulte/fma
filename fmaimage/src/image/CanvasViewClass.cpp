#include <image/CanvasViewClass.hpp>
#include <image/CanvasClass.hpp>
#include <fma/core/Number.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/types/Undefined.hpp>
#include <fma/types/Object.hpp>
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
  template<> const char *getInternalObjectName<CanvasViewPtr>() { return "fmaImageCanvasView"; }
}
}

// ----------------------------------------------------------------------------
ClassPtr CanvasViewClass::create(const ModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("CanvasView", "fmaImage::CanvasView"));
  klass->extends(ClassObject);

  klass->setMember("__deserialize", TypePtr(new InternalFunctionValue("__deserialize", CanvasClass::__deserialize)));

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", CanvasViewClass::initialize)));

  root->setMember("CanvasView", klass);
  return klass;
}

// ----------------------------------------------------------------------------
TypePtr CanvasViewClass::createInstance(const interpret::ContextPtr &context, const types::TypePtr &parent, uint32_t left, uint32_t top, uint32_t width, uint32_t height) {
  ClassPtr CanvasView = context->getRootLevelContext()->getMember("fmaImage")->get()->getMember("CanvasView")->asClass();
  if (!CanvasView) {
    TypePtr result(new Undefined());
    return result;
  }

  GroupedParameterList params;
  params.push_back(parent);
  params.push_back(NumberClass::createInstance(context, left)->get());
  params.push_back(NumberClass::createInstance(context, top)->get());
  params.push_back(NumberClass::createInstance(context, width)->get());
  params.push_back(NumberClass::createInstance(context, height)->get());
  return CanvasView->createInstance(context, params);
}

// ----------------------------------------------------------------------------
ResultPtr CanvasViewClass::initialize(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (args.size() != 5) {
    return ResultPtr(new Result());
  }
  
  const TypePtr &parent = args.at(0);
  int64_t left = args.at(1)->convertToNumber(context);
  int64_t top = args.at(2)->convertToNumber(context);
  int64_t width = args.at(3)->convertToNumber(context);
  int64_t height = args.at(4)->convertToNumber(context);

  if (parent->hasMember("__canvas_view")) {
    context->self()->setMember("__canvas_view", TypePtr(
      new InternalCanvasViewValue(CanvasViewPtr(
        new CanvasView(canvasView(context->getProject(), parent), left, top, width, height)
      ))
    ));
  } else if (parent->hasMember("__canvas")) {
    context->self()->setMember("__canvas_view", TypePtr(
      new InternalCanvasViewValue(CanvasViewPtr(
        new CanvasView(CanvasClass::canvas(context->getProject(), parent), left, top, width, height)
      ))
    ));
  } else {
    return ResultPtr(new Result());
  }

  return Result::executed(context, context->self());
}

// ----------------------------------------------------------------------------
const CanvasViewPtr &CanvasViewClass::canvasView(const interpret::ContextPtr &context) {
  return canvasView(context->getProject(), context->self());
}

// ----------------------------------------------------------------------------
const CanvasViewPtr &CanvasViewClass::canvasView(Project *project, const types::TypePtr &value) {
  const TypePtr &obj = value->getMember("__canvas_view");
  if (!obj || !obj->isInternalObjectOfType("fmaImageCanvasView")) {
    project->log().error() << "Unable to access canvas view";
    return NULL;
  }

  return std::dynamic_pointer_cast<InternalCanvasViewValue>(obj)->getValue();
}

// ----------------------------------------------------------------------------
