#include <image/BaseCanvasClass.hpp>
#include <image/CanvasArrayClass.hpp>
#include <image/CanvasClass.hpp>
#include <image/CanvasViewClass.hpp>
#include <image/PaletteClass.hpp>
#include <image/PixelClass.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/types/Undefined.hpp>
#include <fma/types/Object.hpp>
#include <fma/core/Array.hpp>
#include <fma/core/Number.hpp>
#include <fma/core/SerializerBuffer.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/Project.hpp>
#include <iostream>

using namespace FMA;
using namespace FMA::types;
using namespace FMA::core;
using namespace FMA::interpret;
using namespace FMA::image;

// ----------------------------------------------------------------------------
ClassPtr BaseCanvasClass::create(const ModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("BaseCanvas", "fmaImage::BaseCanvas"));
  klass->extends(ClassObject);

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("width", TypePtr(new InternalFunctionValue("width", BaseCanvasClass::width)));
  proto->setMember("height", TypePtr(new InternalFunctionValue("height", BaseCanvasClass::height)));
  proto->setMember("rows", TypePtr(new InternalFunctionValue("rows", BaseCanvasClass::rows)));
  proto->setMember("cols", TypePtr(new InternalFunctionValue("cols", BaseCanvasClass::cols)));
  proto->setMember("pixels", TypePtr(new InternalFunctionValue("pixels", BaseCanvasClass::pixels)));
  proto->setMember("chunk", TypePtr(new InternalFunctionValue("chunk", BaseCanvasClass::chunk)));
  proto->setMember("required_palette", TypePtr(new InternalFunctionValue("required_palette", BaseCanvasClass::required_palette)));
  proto->setMember("apply_palette", TypePtr(new InternalFunctionValue("apply_palette", BaseCanvasClass::apply_palette)));
  proto->setMember("dump", TypePtr(new InternalFunctionValue("dump", BaseCanvasClass::dump)));
  proto->setMember("__serialize", TypePtr(new InternalFunctionValue("__serialize", BaseCanvasClass::__serialize)));

  root->setMember("BaseCanvas", klass);
  return klass;
}

// ----------------------------------------------------------------------------
ResultPtr BaseCanvasClass::width(const ContextPtr &context, const GroupedParameterList &) {
  BaseCanvasAccessorPtr canvas(allocateAccessor(context));
  if (!canvas) {
    return ResultPtr(new Result());
  }

  return NumberClass::createInstance(context, canvas->width());
}

// ----------------------------------------------------------------------------
ResultPtr BaseCanvasClass::height(const ContextPtr &context, const GroupedParameterList &) {
  BaseCanvasAccessorPtr canvas(allocateAccessor(context));
  if (!canvas) {
    return ResultPtr(new Result());
  }

  return NumberClass::createInstance(context, canvas->height());
}


// ----------------------------------------------------------------------------
ResultPtr BaseCanvasClass::apply_palette(const ContextPtr &context, const GroupedParameterList &parameter) {
  BaseCanvasAccessorPtr canvas(allocateAccessor(context));
  if (!canvas) {
    return ResultPtr(new Result());
  }

  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    return ResultPtr(new Result());
  }

  PalettePtr palette = PaletteClass::palette(context->getProject(), args.front());
  if (!palette) {
    return ResultPtr(new Result());
  }

  CanvasPtr newCanvas(canvas->clone());
  newCanvas->applyPalette(palette);

  TypePtr self = context->self();
  self->setMember("__canvas", TypePtr(new InternalCanvasValue(newCanvas)));
  self->removeMember("__canvas_view");
  self->removeMember("__palette");

  return Result::executed(context, self);
}

// ----------------------------------------------------------------------------
ResultPtr BaseCanvasClass::dump(const ContextPtr &context, const GroupedParameterList &) {
  BaseCanvasAccessorPtr canvas(allocateAccessor(context));
  if (!canvas) {
    return ResultPtr(new Result());
  }

  canvas->dump();

  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
ResultPtr BaseCanvasClass::required_palette(const ContextPtr &context, const GroupedParameterList &) {
  BaseCanvasAccessorPtr canvas(allocateAccessor(context));
  if (!canvas) {
    return ResultPtr(new Result());
  }

  PalettePtr newPalette(new Palette());
  std::map<uint32_t, uint16_t> colorMap;
  std::map<uint32_t, uint16_t>::iterator it;

  // First color always is transparent color
  newPalette->add(Color(0, 0, 0, 0));

  uint32_t width = canvas->width();
  uint32_t height = canvas->height();
  for (uint32_t x=0; x<width; x++) {
    for (uint32_t y=0; y<height; y++) {
      const Color &color = canvas->get(x, y);

      if (color.isTransparent()) {
        continue;
      }

      uint32_t hash = color.toUint32(false);
      it = colorMap.find(hash);
      if (it == colorMap.end()) {
        colorMap[hash] = newPalette->getNumColors();
        newPalette->add(color);
      }
    }
  }

  TypePtr palette = PaletteClass::createInstance(context);
  palette->setMember("__palette", TypePtr(new InternalPaletteValue(newPalette)));
  return Result::executed(context, palette);
}

// ----------------------------------------------------------------------------
ResultPtr BaseCanvasClass::rows(const ContextPtr &context, const GroupedParameterList &) {
  BaseCanvasAccessorPtr canvas(allocateAccessor(context));
  if (!canvas) {
    return ResultPtr(new Result());
  }

  std::vector<TypePtr> list;
  uint32_t width = canvas->width();
  uint32_t height = canvas->height();
  for (uint32_t i=0; i<height; i++) {
    list.push_back(CanvasViewClass::createInstance(context, context->self(), 0, i, width, 1));
  }
  return CanvasArrayClass::createInstance(context, list);
}

// ----------------------------------------------------------------------------
ResultPtr BaseCanvasClass::cols(const ContextPtr &context, const GroupedParameterList &) {
  BaseCanvasAccessorPtr canvas(allocateAccessor(context));
  if (!canvas) {
    return ResultPtr(new Result());
  }

  std::vector<TypePtr> list;
  uint32_t width = canvas->width();
  uint32_t height = canvas->height();
  for (uint32_t i=0; i<width; i++) {
    list.push_back(CanvasViewClass::createInstance(context, context->self(), i, 0, 1, height));
  }
  return CanvasArrayClass::createInstance(context, list);
}

// ----------------------------------------------------------------------------
ResultPtr BaseCanvasClass::pixels(const ContextPtr &context, const GroupedParameterList &) {
  BaseCanvasAccessorPtr canvas(allocateAccessor(context));
  if (!canvas) {
    return ResultPtr(new Result());
  }

  std::vector<TypePtr> list;
  uint32_t width = canvas->width();
  uint32_t height = canvas->height();
  for (uint32_t x=0; x<width; x++) {
    for (uint32_t y=0; y<height; y++) {
      list.push_back(PixelClass::createInstance(context, context->self(), x, y));
    }
  }

  return ArrayClass::createInstance(context, list);
}

// ----------------------------------------------------------------------------
ResultPtr BaseCanvasClass::chunk(const ContextPtr &context, const GroupedParameterList &parameter) {
  BaseCanvasAccessorPtr canvas(allocateAccessor(context));
  if (!canvas) {
    return ResultPtr(new Result());
  }

  const TypeList &args = parameter.only_args();
  if (args.size() < 2) {
    return ResultPtr(new Result());
  }

  TypePtr self = context->self();
  TypeList list;
  uint32_t width = canvas->width();
  uint32_t height = canvas->height();
  uint32_t chunkWidth = args.at(0)->convertToNumber(context);
  uint32_t chunkHeight = args.at(1)->convertToNumber(context);
  
  for (uint32_t y=0; y<height; y+=chunkHeight) {
    for (uint32_t x=0; x<width; x+=chunkWidth) {
      int32_t localWidth = std::min(width, x + chunkWidth) - x;
      int32_t localHeight = std::min(height, y + chunkHeight) - y;
      if (localWidth <= 0 || localHeight <= 0) {
        continue;
      }

      list.push_back(CanvasViewClass::createInstance(context, self, x, y, localWidth, localHeight));
    }
  }

  return CanvasArrayClass::createInstance(context, list);
}

// ----------------------------------------------------------------------------
ResultPtr BaseCanvasClass::__serialize(const ContextPtr &context, const GroupedParameterList &parameter) {
  const auto &bufferPtr = SerializerBufferClass::getBuffer(context->getProject(), parameter.only_args().front());
  const auto &buffer = bufferPtr.lock();
  if (!buffer) {
    return ResultPtr(new Result());
  }
  
  auto imagePtr = allocateAccessor(context);
  if (!imagePtr) {
    return ResultPtr(new Result());
  }

  auto canvas = imagePtr->clone();
  uint32_t width = canvas->getWidth();
  uint32_t height = canvas->getHeight();
  uint8_t bpp = canvas->getBytesPerPixel();

  buffer->write(&width, sizeof(width));
  buffer->write(&height, sizeof(height));
  buffer->write(&bpp, sizeof(bpp));
  
  if (bpp == 1) {
    auto palette = canvas->getPalette();
    uint16_t numColors = palette->getNumColors();
    buffer->write(&numColors, sizeof(numColors));
    for (uint16_t i=0; i<numColors; i++) {
      uint32_t color = palette->getColor(i).toUint32();
      buffer->write(&color, sizeof(color));
    }
  } else {
    uint16_t numColors = 0;
    buffer->write(&numColors, sizeof(numColors));
  }

  buffer->write(canvas->getData(), height * width * bpp);

  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
BaseCanvasAccessorPtr BaseCanvasClass::allocateAccessor(const ContextPtr &context) {
  return allocateAccessor(context->getProject(), context->self());
}

// ----------------------------------------------------------------------------
BaseCanvasAccessorPtr BaseCanvasClass::allocateAccessor(Project *project, const types::TypePtr &value) {
  if (value->hasMember("__canvas")) {
    return BaseCanvasAccessorPtr(new CanvasAccessor(CanvasClass::canvas(project, value)));
  } else if (value->hasMember("__canvas_view")) {
    return BaseCanvasAccessorPtr(new CanvasViewAccessor(CanvasViewClass::canvasView(project, value)));
  } else if (value->hasMember("__palette")) {
    return BaseCanvasAccessorPtr(new PaletteAccessor(PaletteClass::palette(project, value)));
  } else {
    return BaseCanvasAccessorPtr();
  }
}
