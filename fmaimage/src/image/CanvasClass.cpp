#include <image/CanvasClass.hpp>
#include <image/CanvasViewClass.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/types/Undefined.hpp>
#include <fma/types/Object.hpp>
#include <fma/core/DeserializerBuffer.hpp>
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
  template<> const char *getInternalObjectName<CanvasPtr>() { return "fmaImageCanvas"; }
}
}

// ----------------------------------------------------------------------------
ClassPtr CanvasClass::create(const ModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("Canvas", "fmaImage::Canvas"));
  klass->extends(ClassObject);
  klass->setMember("__deserialize", TypePtr(new InternalFunctionValue("__deserialize", CanvasClass::__deserialize)));

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", CanvasClass::initialize)));

  root->setMember("Canvas", klass);
  return klass;
}

// ----------------------------------------------------------------------------
TypePtr CanvasClass::createInstance(const interpret::ContextPtr &context) {
  ClassPtr Canvas = context->getRootLevelContext()->getMember("fmaImage")->get()->getMember("Canvas")->asClass();
  if (!Canvas) {
    TypePtr result(new Undefined());
    return result;
  }

  GroupedParameterList emptyParams;
  return Canvas->createInstance(context, emptyParams);
}

// ----------------------------------------------------------------------------
ResultPtr CanvasClass::initialize(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  CanvasPtr canvas;

  switch (args.size()) {
    case 0:
    case 1:
      canvas = CanvasPtr(new Canvas());
      break;

    default:
    case 2:
      canvas = CanvasPtr(Canvas::create(
        args.at(0)->convertToNumber(context),
        args.at(1)->convertToNumber(context)
      ));
      break;
  }

  context->self()->setMember("__canvas", TypePtr(new InternalCanvasValue(canvas)));

  return Result::executed(context, context->self());
}

// ----------------------------------------------------------------------------
ResultPtr CanvasClass::__deserialize(const ContextPtr &context, const GroupedParameterList &parameter) {
  const auto &bufferPtr = DeserializerBufferClass::getBuffer(context->getProject(), parameter.only_args().front());
  const auto &buffer = bufferPtr.lock();
  if (!buffer) {
    return ResultPtr(new Result());
  }
  
  uint32_t width;
  uint32_t height;
  uint8_t bpp;

  buffer->read(&width, sizeof(width));
  buffer->read(&height, sizeof(height));
  buffer->read(&bpp, sizeof(bpp));

  uint16_t numColors;
  PalettePtr palette;
  buffer->read(&numColors, sizeof(numColors));
  if (numColors > 0) {
    palette = PalettePtr(new Palette());

    for (uint16_t i=0; i<numColors; i++) {
      uint32_t colorValue;
      buffer->read(&colorValue, sizeof(colorValue));
      palette->add(Color::fromUint32(colorValue));
    }
  }
  
  uint32_t size = width * height * bpp;
  unsigned char *data = (unsigned char*)malloc(size);
  if (buffer->read(data, size) < size) {
    free(data);
    return ResultPtr(new Result());
  }

  CanvasPtr canvas(Canvas::createRaw(width, height, bpp, palette, data));
  TypePtr canvasType(new InternalCanvasValue(canvas));

  TypePtr result(createInstance(context));
  result->setMember("__canvas", canvasType);

  buffer->store(result);

  return Result::executed(context, result);
}

// ----------------------------------------------------------------------------
const CanvasPtr &CanvasClass::canvas(const interpret::ContextPtr &context) {
  return canvas(context->getProject(), context->self());
}

// ----------------------------------------------------------------------------
const CanvasPtr &CanvasClass::canvas(Project *project, const types::TypePtr &value) {
  const TypePtr &obj = value->getMember("__canvas");
  if (!obj || !obj->isInternalObjectOfType("fmaImageCanvas")) {
    project->log().error() << "Unable to access canvas";
    return NULL;
  }

  return std::dynamic_pointer_cast<InternalCanvasValue>(obj)->getValue();
}

// ----------------------------------------------------------------------------
