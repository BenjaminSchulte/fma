#include <image/PixelClass.hpp>
#include <image/BaseCanvasClass.hpp>
#include <fma/core/Number.hpp>
#include <fma/core/String.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/types/Undefined.hpp>
#include <fma/types/Object.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/Project.hpp>
#include <sstream>
#include <iomanip>

using namespace FMA;
using namespace FMA::types;
using namespace FMA::interpret;
using namespace FMA::image;
using namespace FMA::core;

// ----------------------------------------------------------------------------
ClassPtr PixelClass::create(const ModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("Pixel", "fmaImage::Pixel"));
  klass->extends(ClassObject);

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", PixelClass::initialize)));
  proto->setMember("to_s", TypePtr(new InternalFunctionValue("to_s", PixelClass::to_s)));

  root->setMember("Pixel", klass);
  return klass;
}

// ----------------------------------------------------------------------------
TypePtr PixelClass::createInstance(const interpret::ContextPtr &context, const types::TypePtr &parent, uint32_t left, uint32_t top) {
  ClassPtr Pixel = context->getRootLevelContext()->getMember("fmaImage")->get()->getMember("Pixel")->asClass();
  if (!Pixel) {
    TypePtr result(new Undefined());
    return result;
  }

  GroupedParameterList params;
  params.push_back(parent);
  params.push_back(NumberClass::createInstance(context, left)->get());
  params.push_back(NumberClass::createInstance(context, top)->get());
  return Pixel->createInstance(context, params);
}

// ----------------------------------------------------------------------------
ResultPtr PixelClass::initialize(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (args.size() != 3) {
    return ResultPtr(new Result());
  }
  
  const TypePtr &parent = args.at(0);
  int64_t left = args.at(1)->convertToNumber(context);
  int64_t top = args.at(2)->convertToNumber(context);

  BaseCanvasAccessorPtr canvas(BaseCanvasClass::allocateAccessor(context->getProject(), parent));
  Color color = canvas->get(left, top);

  context->self()->setMember("__parent", parent);
  context->self()->setMember("x", args.at(1));
  context->self()->setMember("y", args.at(2));
  context->self()->setMember("r", NumberClass::createInstance(context, color.r)->get());
  context->self()->setMember("g", NumberClass::createInstance(context, color.g)->get());
  context->self()->setMember("b", NumberClass::createInstance(context, color.b)->get());
  context->self()->setMember("a", NumberClass::createInstance(context, color.a)->get());

  return Result::executed(context, context->self());
}

// ----------------------------------------------------------------------------
ResultPtr PixelClass::to_s(const ContextPtr &context, const GroupedParameterList &) {
  std::ostringstream os;
  os << "#";
  os << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)context->self()->getMember("r")->convertToNumber(context);
  os << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)context->self()->getMember("g")->convertToNumber(context);
  os << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)context->self()->getMember("b")->convertToNumber(context);

  return StringClass::createInstance(context, os.str());
}
