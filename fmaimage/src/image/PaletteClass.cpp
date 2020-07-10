#include <image/PaletteClass.hpp>
#include <image/PaletteArrayClass.hpp>
#include <image/PixelClass.hpp>
#include <image/CanvasAccessor.hpp>
#include <fma/core/Nil.hpp>
#include <fma/core/Array.hpp>
#include <fma/core/SerializerBuffer.hpp>
#include <fma/core/DeserializerBuffer.hpp>
#include <fma/core/Number.hpp>
#include <fma/types/Undefined.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/types/Object.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/Project.hpp>
#include <iostream>

using namespace FMA;
using namespace FMA::types;
using namespace FMA::interpret;
using namespace FMA::image;
using namespace FMA::core;

namespace {
  PalettePtr emptyPalette;
}

namespace FMA {
namespace types {
  template<> const char *getInternalObjectName<PalettePtr>() { return "fmaImagePalette"; }
}
}

// ----------------------------------------------------------------------------
ClassPtr PaletteClass::create(const ModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("Palette", "fmaImage::Palette"));
  klass->extends(ClassObject);
  klass->setMember("__deserialize", TypePtr(new InternalFunctionValue("__deserialize", PaletteClass::__deserialize)));

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", PaletteClass::initialize)));
  proto->setMember("add", TypePtr(new InternalFunctionValue("add", PaletteClass::add)));
  proto->setMember("colors", TypePtr(new InternalFunctionValue("colors", PaletteClass::colors)));
  proto->setMember("chunk", TypePtr(new InternalFunctionValue("chunk", PaletteClass::chunk)));
  proto->setMember("dump", TypePtr(new InternalFunctionValue("dump", PaletteClass::dump)));
  proto->setMember("num_colors", TypePtr(new InternalFunctionValue("num_colors", PaletteClass::num_colors)));
  proto->setMember("size", TypePtr(new InternalFunctionValue("size", PaletteClass::num_colors)));
  proto->setMember("length", TypePtr(new InternalFunctionValue("length", PaletteClass::num_colors)));
  proto->setMember("__serialize", TypePtr(new InternalFunctionValue("__serialize", PaletteClass::__serialize)));

  root->setMember("Palette", klass);
  return klass;
}

// ----------------------------------------------------------------------------
TypePtr PaletteClass::createInstance(const interpret::ContextPtr &context) {
  ClassPtr Palette = context->getRootLevelContext()->getMember("fmaImage")->get()->getMember("Palette")->asClass();
  if (!Palette) {
    TypePtr result(new Undefined());
    return result;
  }

  GroupedParameterList emptyParams;
  return Palette->createInstance(context, emptyParams);
}

// ----------------------------------------------------------------------------
ResultPtr PaletteClass::initialize(const ContextPtr &context, const GroupedParameterList &) {
  TypePtr self = context->self();

  PalettePtr palette(new Palette());
  context->self()->setMember("__palette", TypePtr(new InternalPaletteValue(palette)));
  
  return Result::executed(context, self);
}

// ----------------------------------------------------------------------------
ResultPtr PaletteClass::add(const ContextPtr &context, const GroupedParameterList &parameter) {
  TypePtr self = context->self();
  const PalettePtr &pal = palette(context);

  const TypeList &args = parameter.only_args();
  if (args.size() < 3) {
    return Result::executed(context, self);
  }

  uint8_t r = args.at(0)->convertToNumber(context);
  uint8_t g = args.at(1)->convertToNumber(context);
  uint8_t b = args.at(2)->convertToNumber(context);
  uint8_t a = 255;

  if (args.size() >= 4) {
    a = args.at(3)->convertToNumber(context);
  }

  pal->add(Color(r, g, b, a));

  return Result::executed(context, self);
}

// ----------------------------------------------------------------------------
ResultPtr PaletteClass::colors(const ContextPtr &context, const GroupedParameterList &) {
  TypeList colors;
  TypePtr self = context->self();
  PalettePtr pal(palette(context));

  for (uint16_t i=0; i<pal->getNumColors(); i++) {
    colors.push_back(PixelClass::createInstance(context, self, i, 0));
  }

  return ArrayClass::createInstance(context, colors);
}

// ----------------------------------------------------------------------------
ResultPtr PaletteClass::chunk(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    return ResultPtr(new Result());
  }

  PalettePtr pal(palette(context));
  TypeList palettes;

  uint16_t colorsLeft = pal->getNumColors();
  uint16_t offset = 0;
  uint16_t size;
  uint16_t chunkSize = args.front()->convertToNumber(context);
  while (colorsLeft > 0) {
    TypePtr newPalette = createInstance(context);

    size = std::min(colorsLeft, chunkSize);
    newPalette->setMember("__palette", TypePtr(new InternalPaletteValue(pal->slice(offset, size))));
    offset += size;
    colorsLeft -= size;

    palettes.push_back(newPalette);
  }

  return PaletteArrayClass::createInstance(context, palettes);
}

// ----------------------------------------------------------------------------
ResultPtr PaletteClass::num_colors(const ContextPtr &context, const GroupedParameterList &) {
  PalettePtr pal(palette(context));

  return NumberClass::createInstance(context, pal->getNumColors());
}

// ----------------------------------------------------------------------------
ResultPtr PaletteClass::dump(const ContextPtr &context, const GroupedParameterList &) {
  PalettePtr pal(palette(context));
  PaletteAccessor access(pal);
  access.dump();

  return Result::executed(context, context->self());
}

// ----------------------------------------------------------------------------
ResultPtr PaletteClass::__serialize(const ContextPtr &context, const GroupedParameterList &parameter) {
  const auto &bufferPtr = SerializerBufferClass::getBuffer(context->getProject(), parameter.only_args().front());
  const auto &buffer = bufferPtr.lock();
  if (!buffer) {
    return ResultPtr(new Result());
  }
  
  auto palettePtr = palette(context);
  if (!palettePtr) {
    return ResultPtr(new Result());
  }
  uint16_t numColors = palettePtr->getNumColors();
  buffer->write(&numColors, sizeof(numColors));
  for (uint16_t i=0; i<numColors; i++) {
    uint32_t color = palettePtr->getColor(i).toUint32();
    buffer->write(&color, sizeof(color));
  }

  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
ResultPtr PaletteClass::__deserialize(const ContextPtr &context, const GroupedParameterList &parameter) {
  const auto &bufferPtr = DeserializerBufferClass::getBuffer(context->getProject(), parameter.only_args().front());
  const auto &buffer = bufferPtr.lock();
  if (!buffer) {
    return ResultPtr(new Result());
  }
  
  uint16_t numColors;
  PalettePtr palette;
  buffer->read(&numColors, sizeof(numColors));
  palette = PalettePtr(new Palette());

  for (uint16_t i=0; i<numColors; i++) {
    uint32_t colorValue;
    buffer->read(&colorValue, sizeof(colorValue));
    palette->add(Color::fromUint32(colorValue));
  }
  
  TypePtr result(createInstance(context));
  result->setMember("__palette", TypePtr(new InternalPaletteValue(palette)));

  buffer->store(result);

  return Result::executed(context, result);
}

// ----------------------------------------------------------------------------
const PalettePtr &PaletteClass::palette(const interpret::ContextPtr &context) {
  return palette(context->getProject(), context->self());
}

// ----------------------------------------------------------------------------
const PalettePtr &PaletteClass::palette(Project *project, const types::TypePtr &value) {
  const TypePtr &obj = value->getMember("__palette");
  if (!obj || !obj->isInternalObjectOfType("fmaImagePalette")) {
    project->log().error() << "Unable to access palette";
    return emptyPalette;
  }

  return std::dynamic_pointer_cast<InternalPaletteValue>(obj)->getValue();
}

// ----------------------------------------------------------------------------
