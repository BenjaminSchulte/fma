#include <memory/MemoryMapClass.hpp>
#include <memory/MemoryMapBankSection.hpp>
#include <memory/MemoryAdapter.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/types/Object.hpp>
#include <fma/core/Number.hpp>
#include <fma/Project.hpp>

#include <iostream>

using namespace FMA;
using namespace FMA::memory;
using namespace FMA::types;
using namespace FMA::interpret;

typedef InternalObject<class MemoryMap*> InternalMemoryMapValue;

namespace FMA {
namespace types {
  template<> const char *getInternalObjectName<class MemoryMap*>() { return "MemoryMap"; }
}
}

// ----------------------------------------------------------------------------
ClassPtr MemoryMapClass::create(const RootModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("MemoryMap", "MemoryMap"));
  klass->extends(ClassObject);

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", MemoryMapClass::initialize)));
  proto->setMember("used_for_dynamic_data", TypePtr(new InternalFunctionValue("used_for_dynamic_data", MemoryMapClass::used_for_dynamic_data)));
  proto->setMember("used_for_static_data", TypePtr(new InternalFunctionValue("used_for_static_data", MemoryMapClass::used_for_static_data)));
  proto->setMember("no_banks", TypePtr(new InternalFunctionValue("no_banks", MemoryMapClass::no_banks)));
  proto->setMember("has_banks", TypePtr(new InternalFunctionValue("has_banks", MemoryMapClass::has_banks)));
  proto->setMember("banks", TypePtr(new InternalFunctionValue("banks", MemoryMapClass::banks)));
  proto->setMember("address", TypePtr(new InternalFunctionValue("address", MemoryMapClass::address)));
  proto->setMember("dump", TypePtr(new InternalFunctionValue("dump", MemoryMapClass::dump)));
  proto->setMember("include_object", TypePtr(new InternalFunctionValue("include_object", MemoryMapClass::include_object)));

  root->setMember("MemoryMap", klass);
  return klass;
}

// ----------------------------------------------------------------------------
ResultPtr MemoryMapClass::initialize(const ContextPtr &context, const GroupedParameterList &) {
  MemoryMap *map = dynamic_cast<MemoryAdapter*>(context->getProject()->getMemoryAdapter())->createMemoryMap();
  context->self()->setMember("__memory_map", TypePtr(new InternalMemoryMapValue(map)));
  return Result::executed(context, context->self());
}

// ----------------------------------------------------------------------------
ResultPtr MemoryMapClass::used_for_dynamic_data(const ContextPtr &context, const GroupedParameterList &) {
  MemoryMap *map = asMemoryMap(context);
  if (map) {
    map->useForDynamicMemory();
  }
  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
ResultPtr MemoryMapClass::used_for_static_data(const ContextPtr &context, const GroupedParameterList &) {
  MemoryMap *map = asMemoryMap(context);
  if (map) {
    map->useForStaticMemory();
  }
  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
ResultPtr MemoryMapClass::include_object(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    context->log().error() << "Missing range or size argument";
    return ResultPtr(new Result());
  }



  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
ResultPtr MemoryMapClass::no_banks(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    context->log().error() << "Missing range or size argument";
    return ResultPtr(new Result());
  }
  
  MemoryMap *map = asMemoryMap(context);
  const TypePtr &object = args.front();
  if (object->isObjectOfType("Range")) {
    const auto &range = object->convertToRange(context);
    map->createBank(0, range.first, range.last - range.first + 1);
  } else {
    map->createBank(0, 0, object->convertToNumber(context));
  }

  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
ResultPtr MemoryMapClass::has_banks(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  const TypeMap &kwargs = parameter.only_kwargs();
  if (!args.size()) {
    context->log().error() << "Missing banks";
    return ResultPtr(new Result());
  }

  TypeMap::const_iterator it;
  MemoryBankSize start=0, size;
  MemoryBankIndex first, last;
  if ((it = kwargs.find("bank_size")) == kwargs.end()) {
    context->log().error() << "Missing bank_size";
    return ResultPtr(new Result());
  }

  size = it->second->convertToNumber(context);

  const TypePtr &object = args.front();
  if (object->isObjectOfType("Range")) {
    const auto &range = object->convertToRange(context);
    first = range.first;
    last = range.last;
  } else {
    first = last = object->convertToNumber(context);
  }

  MemoryMap *map = asMemoryMap(context);
  uint32_t index=first;
  for (; index<=last; index++) {
    map->createBank(index, start, size);
  }

  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
ResultPtr MemoryMapClass::banks(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  const TypeMap &kwargs = parameter.only_kwargs();
  if (!args.size()) {
    context->log().error() << "Missing banks";
    return ResultPtr(new Result());
  }

  TypeMap::const_iterator it;
  MemoryBankIndex first, last;
  MemoryBankSize start=0, size;
  if ((it = kwargs.find("address")) == kwargs.end()) {
    context->log().error() << "Missing address";
    return ResultPtr(new Result());
  }

  if (!it->second->isObjectOfType("Range")) {
    context->log().error() << "Address must be a range";
    return ResultPtr(new Result());
  }

  const auto &range = it->second->convertToRange(context);
  start = range.first;
  size = range.last - start + 1;

  const TypePtr &object = args.front();
  if (object->isObjectOfType("Range")) {
    const auto &range = object->convertToRange(context);
    first = range.first;
    last = range.last;
  } else {
    first = last = object->convertToNumber(context);
  }

  bool hasLocation = false;
  uint64_t locatedAt = 0;
  uint64_t locatedAtStep = size;
  if ((it = kwargs.find("located_at")) != kwargs.end()) {
    hasLocation = true;
    locatedAt = it->second->convertToNumber(context);
  }
  if ((it = kwargs.find("increment_location_by")) != kwargs.end()) {
    locatedAtStep = it->second->convertToNumber(context);
  }

  bool hasShadow = false;
  MemoryBankIndex shadowFromBank = 0;
  MemoryBankSize shadowFromAddress = 0;
  if ((it = kwargs.find("shadows_banks_from")) != kwargs.end()) {
    hasShadow = true;
    shadowFromBank = it->second->convertToNumber(context);
  }
  if ((it = kwargs.find("shadows_addresses_from")) != kwargs.end()) {
    hasShadow = true;
    shadowFromAddress = it->second->convertToNumber(context);
  }

  MemoryMap *map = asMemoryMap(context);
  uint32_t index=first;
  for (; index<=last; index++) {
    MemoryMapBank *bank = map->getBank(index);
    if (!bank) {
      continue;
    }

    MemoryMapBankSection *section = bank->createSection(start, size);
    if (hasLocation) {
      section->setLocatedAt(locatedAt);
      locatedAt += locatedAtStep;
    }

    if (hasShadow) {
      section->setIsShadow(map->createShadow(index, start, shadowFromBank, shadowFromAddress, size));
      shadowFromBank++;
    }
  }

  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
ResultPtr MemoryMapClass::address(const ContextPtr &context, const GroupedParameterList &parameter) {
  GroupedParameterList copy;
  copy.push_back(Parameter(FMA::core::NumberClass::createInstance(context, 0)->get()));

  for (auto &item : parameter.only_args()) {
    copy.push_back(Parameter("address", item));
  }

  for (auto &item : parameter.only_kwargs()) {
    copy.push_back(Parameter(item.first, item.second));
  }

  return banks(context, copy);
}

// ----------------------------------------------------------------------------
ResultPtr MemoryMapClass::dump(const ContextPtr &context, const GroupedParameterList &) {
  asMemoryMap(context)->dump();
  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
MemoryMap *MemoryMapClass::asMemoryMap(const ContextPtr &context) {
  return asMemoryMap(context->getProject(), context->self());
}

// ----------------------------------------------------------------------------
MemoryMap *MemoryMapClass::asMemoryMap(Project *project, const TypePtr &type) {
  const TypePtr &obj = type->getMember("__memory_map");
  if (!obj || !obj->isInternalObjectOfType("MemoryMap")) {
    project->log().error() << "Unable to access internal memory map";
    return NULL;
  }

  return std::dynamic_pointer_cast<InternalMemoryMapValue>(obj)->getValue();
}
