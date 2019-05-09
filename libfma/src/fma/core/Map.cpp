#include <fma/core/Class.hpp>
#include <fma/core/Map.hpp>
#include <fma/core/Boolean.hpp>
#include <fma/core/Number.hpp>
#include <fma/core/String.hpp>
#include <fma/core/Array.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/types/Object.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/ObjectContext.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/Parameter.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/serialize/SerializeTypes.hpp>
#include <fma/Project.hpp>

using namespace FMA;
using namespace FMA::core;
using namespace FMA::types;
using namespace FMA::interpret;

#include <iostream>
#include <sstream>
#include <cmath>

TypeMap __emptyMap;

// ----------------------------------------------------------------------------
ClassPtr MapClass::create(const RootModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("Map", "Map"));
  klass->extends(ClassObject);

  klass->setMember("@FMA_TYPE_ID", TypePtr(new InternalNumberValue(serialize::TYPE_MAP)));

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("each", TypePtr(new InternalFunctionValue("each", MapClass::each)));
  proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", MapClass::initialize)));
  proto->setMember("key?", TypePtr(new InternalFunctionValue("key?", MapClass::key_qm)));
  proto->setMember("size", TypePtr(new InternalFunctionValue("size", MapClass::size)));
  proto->setMember("keys", TypePtr(new InternalFunctionValue("keys", MapClass::keys)));
  proto->setMember("values", TypePtr(new InternalFunctionValue("values", MapClass::values_call)));
  proto->setMember("to_s", TypePtr(new InternalFunctionValue("to_s", MapClass::to_s)));
  
  proto->setMember("[]", TypePtr(new InternalFunctionValue("[]", MapClass::op_index)));
  proto->setMember("[]=", TypePtr(new InternalFunctionValue("[]=", MapClass::op_index_set)));

  root->setMember("Map", klass);
  return klass;
}

// ----------------------------------------------------------------------------
ResultPtr MapClass::createInstance(const ContextPtr &context, const TypeMap &value) {
  // Resolve a 
  ClassPtr number = context->getRootLevelContext()->resolve("Map")->asClass();
  if (!number) {
    return ResultPtr(new Result());
  }

  GroupedParameterList parameters;
  parameters.push_back(TypePtr(new InternalMapValue(value)));
  return Result::executed(context, number->createInstance(context, parameters));
}

// ----------------------------------------------------------------------------
ResultPtr MapClass::each(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &blocks = parameter.only_blocks();
  if (!blocks.size()) {
    return ResultPtr(new Result());
  }

  ResultPtr result(new Result());
  TypePtr block = blocks.front();
  const TypeMap &items = context->self()->convertToMap(context);

  for (auto &item : items) {
    GroupedParameterList yieldParams;
    yieldParams.push_back(Parameter(StringClass::createInstance(context, item.first)->get()));
    yieldParams.push_back(item.second);
    result = block->call(context, yieldParams);
  }
  
  return result;
}

// ----------------------------------------------------------------------------
ResultPtr MapClass::initialize(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  
  switch (args.size()) {
    case 0:
      context->self()->setMember("__value", TypePtr(new InternalMapValue()));
      break;
    
    case 1:
    default:
      context->self()->setMember("__value", args.front());
      break;
  }

  return Result::executed(context, context->self());
}

// ----------------------------------------------------------------------------
ResultPtr MapClass::keys(const ContextPtr &context, const GroupedParameterList &) {
  TypeList list;
  for (auto &pair : values(context)) {
    list.push_back(StringClass::createInstance(context, pair.first)->get());
  }
  return ArrayClass::createInstance(context, list);
}

// ----------------------------------------------------------------------------
ResultPtr MapClass::size(const ContextPtr &context, const GroupedParameterList &) {
  return NumberClass::createInstance(context, values(context).size());
}

// ----------------------------------------------------------------------------
ResultPtr MapClass::values_call(const ContextPtr &context, const GroupedParameterList &) {
  TypeList list;
  for (auto &pair : values(context)) {
    list.push_back(pair.second);
  }
  return ArrayClass::createInstance(context, list);
}

// ----------------------------------------------------------------------------
ResultPtr MapClass::key_qm(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (args.size() == 0) {
    return BooleanClass::createInstance(context, false);
  }

  const std::string &key = args.front()->convertToString(context);
  const TypeMap &map = context->self()->convertToMap(context);

  return BooleanClass::createInstance(context, map.find(key) != map.end());
}

// ----------------------------------------------------------------------------
ResultPtr MapClass::to_s(const ContextPtr &context, const GroupedParameterList &) {
  const TypeMap &map = context->self()->convertToMap(context);
  
  std::ostringstream os;
  os << "{";
  for (auto &pair : map) {
    os << pair.first << ":" << pair.second->convertToString(context) << ",";
  }
  os << "}";
  
  return StringClass::createInstance(context, os.str());
}

// ----------------------------------------------------------------------------
ResultPtr MapClass::op_index(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (args.size() == 0) {
    return ResultPtr(new Result());
  }

  const std::string &key = args.front()->convertToString(context);
  const TypeMap &map = context->self()->convertToMap(context);

  ResultPtr result;
  auto it = map.find(key);
  if (it == map.end()) {
    result = ResultPtr(new Result());
  } else {
    result = ResultPtr(new Result(context, it->second));
  }

  result->makeAssignable(
    ContextPtr(new ObjectContext(context->getInterpreter(), context->self())),
    key
  );
  
  return result;
}

// ----------------------------------------------------------------------------
ResultPtr MapClass::op_index_set(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (args.size() < 2) {
    return ResultPtr(new Result());
  }

  const std::string &key = args.front()->convertToString(context);
  TypeMap &map = values(context);
  map[key] = args.back();

  return ResultPtr(new Result(context, args.back()));
}

// ----------------------------------------------------------------------------
InternalMapValue *MapClass::internalMap(const interpret::ContextPtr &context) {
  return internalMap(context, context->self());
}

// ----------------------------------------------------------------------------
InternalMapValue *MapClass::internalMap(const interpret::ContextPtr &context, const TypePtr &self) {
  if (!self->isObjectOfType("Map")) {
    return NULL;
  }

  TypePtr valueObject = self->getMember("__value");
  if (!valueObject->isInternalObjectOfType("Map")) {
    context->log().error() << self->asString() << " is not not an array";
    return NULL;
  }

  return dynamic_cast<InternalMapValue*>(valueObject.get());
}

// ----------------------------------------------------------------------------
TypeMap &MapClass::values(const interpret::ContextPtr &context) {
  InternalMapValue *array = internalMap(context);
  if (array == NULL) {
    return __emptyMap;
  }

  return array->getValue();
}

// ----------------------------------------------------------------------------
void MapClass::serializeObject(const TypePtr &type, const serialize::SerializeObjectPtr &object) {
  uint32_t numItems = 0;

  InternalMapValue *array = internalMap(object->getContext(), type);
  if (array == NULL) {
    object->write(&numItems, sizeof(numItems));
    return;
  }

  const auto &values = array->getValue();
  numItems = values.size();
  object->write(&numItems, sizeof(numItems));

  for (const auto &item : values) {
    object->write(item.first);
    object->write(item.second);
  }
}

// ----------------------------------------------------------------------------
bool MapClass::deserializeObject(const serialize::DeserializeObjectPtr &object) {
  uint32_t numItems = 0;
  
  TypePtr result = createInstance(object->getContext(), TypeMap())->get();
  TypeMap &map = internalMap(object->getContext(), result)->getValue();
  
  object->store(result);
  object->read(&numItems, sizeof(numItems));

  for (uint32_t i=0; i<numItems; i++) {
    std::string key = object->readString();
    map[key] = object->readObject();
  }

  return true;
}
