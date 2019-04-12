#include <image/PaletteArrayClass.hpp>
#include <image/PaletteClass.hpp>
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

// ----------------------------------------------------------------------------
ClassPtr PaletteArrayClass::create(const ModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("PaletteArray", "fmaImage::PaletteArray"));
  klass->extends(ClassObject);

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("find_matching_palettes", TypePtr(new InternalFunctionValue("find_matching_palettes", PaletteArrayClass::find_matching_palettes)));
  proto->setMember("index_of", TypePtr(new InternalFunctionValue("index_of", PaletteArrayClass::index_of)));

  root->setMember("PaletteArray", klass);
  return klass;
}

// ----------------------------------------------------------------------------
ResultPtr PaletteArrayClass::createInstance(const ContextPtr &context, std::vector<types::TypePtr> &list) {
  ClassPtr number = context->getRootLevelContext()->getMember("fmaImage")->get()->getMember("PaletteArray")->asClass();
  if (!number) {
    return ResultPtr(new Result());
  }

  GroupedParameterList parameters;
  parameters.push_back(TypePtr(new InternalArrayValue(list)));
  return ResultPtr(new Result(context, number->createInstance(context, parameters)));
}

// ----------------------------------------------------------------------------
ResultPtr PaletteArrayClass::find_matching_palettes(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    return ResultPtr(new Result(context, context->self()));
  }

  PalettePtr comparePalette = PaletteClass::palette(context->getProject(), args.front());
  if (!comparePalette) {
    context->log().error() << "Invalid parameter. Must be a palette.";
    return ResultPtr(new Result(context, context->self()));
  }

  TypeList &items = ArrayClass::values(context);
  TypeList result;

  for (auto &item: items) {
    const PalettePtr &palette = PaletteClass::palette(context->getProject(), item);

    if (palette->contains(comparePalette)) {
      result.push_back(item);
    }
  }

  return createInstance(context, result);
}

// ----------------------------------------------------------------------------
ResultPtr PaletteArrayClass::index_of(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    return ResultPtr(new Result(context, context->self()));
  }

  PalettePtr comparePalette = PaletteClass::palette(context->getProject(), args.front());
  if (!comparePalette) {
    context->log().error() << "Invalid parameter. Must be a palette.";
    return ResultPtr(new Result(context, context->self()));
  }

  TypeList &items = ArrayClass::values(context);
  int32_t index = 0;
  int32_t result = -1;
  for (auto &item: items) {
    const PalettePtr &palette = PaletteClass::palette(context->getProject(), item);

    if (palette->equals(comparePalette)) {
      result = index;
      break;
    }

    index++; 
  }

  return NumberClass::createInstance(context, result);
}
