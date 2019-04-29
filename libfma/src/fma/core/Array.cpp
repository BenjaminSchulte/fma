#include <fma/core/Class.hpp>
#include <fma/core/Array.hpp>
#include <fma/core/Boolean.hpp>
#include <fma/core/Nil.hpp>
#include <fma/core/Number.hpp>
#include <fma/core/String.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/types/Object.hpp>
#include <fma/types/Undefined.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/Parameter.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/interpret/ObjectContext.hpp>
#include <fma/serialize/SerializeObject.hpp>
#include <fma/Project.hpp>

using namespace FMA;
using namespace FMA::core;
using namespace FMA::types;
using namespace FMA::interpret;
using namespace FMA::serialize;

#include <iostream>
#include <sstream>
#include <cmath>

TypeList __emptyList;

// ----------------------------------------------------------------------------
ClassPtr ArrayClass::create(const RootModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("Array", "Array"));
  klass->extends(ClassObject);

  klass->setMember("@FMA_TYPE_ID", TypePtr(new InternalNumberValue(TYPE_ARRAY)));

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("dup", TypePtr(new InternalFunctionValue("dup", ArrayClass::dup)));
  proto->setMember("count", TypePtr(new InternalFunctionValue("count", ArrayClass::count)));
  proto->setMember("each", TypePtr(new InternalFunctionValue("each", ArrayClass::each)));
  proto->setMember("empty?", TypePtr(new InternalFunctionValue("empty?", ArrayClass::empty_qm)));
  proto->setMember("first", TypePtr(new InternalFunctionValue("first", ArrayClass::first)));
  proto->setMember("include?", TypePtr(new InternalFunctionValue("include?", ArrayClass::include_qm)));
  proto->setMember("index", TypePtr(new InternalFunctionValue("index", ArrayClass::index)));
  proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", ArrayClass::initialize)));
  proto->setMember("last", TypePtr(new InternalFunctionValue("last", ArrayClass::last)));
  proto->setMember("length", TypePtr(new InternalFunctionValue("length", ArrayClass::count)));
  proto->setMember("pop", TypePtr(new InternalFunctionValue("pop", ArrayClass::count)));
  proto->setMember("size", TypePtr(new InternalFunctionValue("size", ArrayClass::count)));
  proto->setMember("map", TypePtr(new InternalFunctionValue("map", ArrayClass::map)));
  proto->setMember("to_s", TypePtr(new InternalFunctionValue("to_s", ArrayClass::to_s)));
  proto->setMember("join", TypePtr(new InternalFunctionValue("join", ArrayClass::join)));

  proto->setMember("+", TypePtr(new InternalFunctionValue("+", ArrayClass::op_add)));
  proto->setMember("<<", TypePtr(new InternalFunctionValue("<<", ArrayClass::op_lshift)));
  proto->setMember("==", TypePtr(new InternalFunctionValue("==", ArrayClass::op_eq)));
  proto->setMember("[]", TypePtr(new InternalFunctionValue("[]", ArrayClass::op_index)));
  proto->setMember("[]=", TypePtr(new InternalFunctionValue("[]=", ArrayClass::op_index_set)));

  root->setMember("Array", klass);
  return klass;
}

// ----------------------------------------------------------------------------
ResultPtr ArrayClass::createInstance(const ContextPtr &context, const TypeList &value) {
  // Resolve a 
  ClassPtr number = context->getRootLevelContext()->resolve("Array")->asClass();
  if (!number) {
    return ResultPtr(new Result());
  }

  GroupedParameterList parameters;
  parameters.push_back(TypePtr(new InternalArrayValue(value)));
  return Result::executed(context, number->createInstance(context, parameters));
}

// ----------------------------------------------------------------------------
ResultPtr ArrayClass::count(const ContextPtr &context, const GroupedParameterList&) {
  const TypeList &items = values(context);

  return NumberClass::createInstance(context, items.size());
}

// ----------------------------------------------------------------------------
ResultPtr ArrayClass::dup(const ContextPtr &context, const GroupedParameterList&) {
  const TypeList &items = values(context);
  return ArrayClass::createInstance(context, items);
}

// ----------------------------------------------------------------------------
ResultPtr ArrayClass::pop(const ContextPtr &context, const GroupedParameterList&) {
  TypeList &items = values(context);
  if (!items.size()) {
    return NilClass::createInstance(context);
  }

  TypePtr last = items.back();
  items.pop_back();

  return Result::executed(context, last);
}

// ----------------------------------------------------------------------------
ResultPtr ArrayClass::each(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &blocks = parameter.only_blocks();
  if (!blocks.size()) {
    return ResultPtr(new Result());
  }

  ResultPtr result(new Result());
  const TypePtr &block = blocks.front();
  const TypeList &items = context->self()->convertToArray(context);

  uint64_t index=0;
  for (auto &item : items) {
    GroupedParameterList yieldParams;
    yieldParams.push_back(Parameter(item));
    yieldParams.push_back(NumberClass::createInstance(context, index++)->get());
    result = block->call(context, yieldParams);
  }
  
  return result;
}

// ----------------------------------------------------------------------------
ResultPtr ArrayClass::map(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &blocks = parameter.only_blocks();
  if (!blocks.size()) {
    return ResultPtr(new Result());
  }

  const TypePtr &block = blocks.front();
  const TypeList &items = context->self()->convertToArray(context);

  TypeList resultList;
  uint64_t index=0;
  for (auto &item : items) {
    GroupedParameterList yieldParams;
    yieldParams.push_back(Parameter(item));
    yieldParams.push_back(NumberClass::createInstance(context, index++)->get());
    resultList.push_back(block->call(context, yieldParams)->get());
  }
  
  return createInstance(context, resultList);
}

// ----------------------------------------------------------------------------
ResultPtr ArrayClass::empty_qm(const ContextPtr &context, const GroupedParameterList &) {
  return BooleanClass::createInstance(context, values(context).size() == 0);
}

// ----------------------------------------------------------------------------
ResultPtr ArrayClass::first(const ContextPtr &context, const GroupedParameterList &) {
  const TypeList &items = values(context);
  if (!items.size()) {
    return ResultPtr(new Result());
  }

  return ResultPtr(new Result(context, items.front()));
}

// ----------------------------------------------------------------------------
ResultPtr ArrayClass::include_qm(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (args.size() < 1) {
    return BooleanClass::createInstance(context, false);
  }

  const TypePtr &self = args.front();
  ContextPtr callContext(new ObjectContext(context->getInterpreter(), self));
  const TypePtr &eq = self->getMember("==");
  
  const TypeList &items = values(context);
  for (auto &item : items) {
    GroupedParameterList params;
    params.push_back(item);
    
    ResultPtr testResult = eq->call(callContext, params);
    if (testResult->get()->convertToBoolean(context)) {
      return testResult;
    }
  }

  return BooleanClass::createInstance(context, false);
}

// ----------------------------------------------------------------------------
ResultPtr ArrayClass::index(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (args.size() < 1) {
    return BooleanClass::createInstance(context, false);
  }

  const TypePtr &self = args.front();
  ContextPtr callContext(new ObjectContext(context->getInterpreter(), self));
  const TypePtr &eq = self->getMember("==");
  
  const TypeList &items = values(context);
  uint32_t index = 0;
  for (auto &item : items) {
    GroupedParameterList params;
    params.push_back(item);
    
    ResultPtr testResult = eq->call(callContext, params);
    if (testResult->get()->convertToBoolean(context)) {
      return NumberClass::createInstance(context, index);
    }

    index++;
  }

  return NilClass::createInstance(context);
}

// ----------------------------------------------------------------------------
ResultPtr ArrayClass::initialize(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  
  switch (args.size()) {
    case 0:
      context->self()->setMember("__value", TypePtr(new InternalArrayValue()));
      break;
    
    case 1:
    default:
      context->self()->setMember("__value", args.front());
      break;
  }

  return Result::executed(context, context->self());
}

// ----------------------------------------------------------------------------
ResultPtr ArrayClass::last(const ContextPtr &context, const GroupedParameterList &) {
  const TypeList &items = values(context);
  if (!items.size()) {
    return ResultPtr(new Result());
  }

  return ResultPtr(new Result(context, items.back()));
}

// ----------------------------------------------------------------------------
ResultPtr ArrayClass::op_eq(const ContextPtr &context, const GroupedParameterList &params) {
  const TypeList &args = params.only_args();
  if (args.size() < 1) {
    return BooleanClass::createInstance(context, false);
  }

  InternalArrayValue *otherObject = internalArray(context, args.front());
  if (!otherObject) {
    return BooleanClass::createInstance(context, false);
  }

  const TypeList &self = values(context);
  const TypeList &other = otherObject->getValue();
  if (self.size() != other.size())  {
    return BooleanClass::createInstance(context, false);
  }

  for (
    TypeList::const_iterator
    itSelf = self.begin(),
    itOther = other.begin();

    itSelf != self.end() && itOther != other.end();

    itSelf++, itOther++
  ) {
    const TypePtr &left = *itSelf;
    const TypePtr &right = *itOther;

    GroupedParameterList params;
    params.push_back(right);

    ContextPtr callContext(new ObjectContext(context->getInterpreter(), left));
    if (!left->getMember("==")->call(callContext, params)->get()->convertToBoolean(context)) {
      return BooleanClass::createInstance(context, false);
    }
  }

  return BooleanClass::createInstance(context, true);
}

// ----------------------------------------------------------------------------
ResultPtr ArrayClass::op_index(const ContextPtr &context, const GroupedParameterList &params) {
  const TypeList &args = params.only_args();
  if (args.size() < 1) {
    return ResultPtr(new Result());
  }

  int64_t index = args.front()->convertToNumber(context);
  const TypeList &items = values(context);
  if (index < 0) {
    index = items.size() + index;

    if (index < 0) {
      return NilClass::createInstance(context);
    }
  }

  ResultPtr result;
  if (index >= (int64_t)items.size()) {
    result = NilClass::createInstance(context);
  } else {
    result = ResultPtr(new Result(context, items[index]));
  }

  result->makeAssignable(
    ContextPtr(new ObjectContext(context->getInterpreter(), context->self())),
    std::to_string(index)
  );

  return result;
}

// ----------------------------------------------------------------------------
ResultPtr ArrayClass::op_index_set(const ContextPtr &context, const GroupedParameterList &params) {
  const TypeList &args = params.only_args();
  if (args.size() < 2) {
    return ResultPtr(new Result());
  }

  TypeList &items = values(context);
  int64_t index = args.front()->convertToNumber(context);
  uint64_t size = items.size();
  if (index < 0) {
    return ResultPtr(new Result());
  }

  while ((uint64_t)index >= size) {
    items.push_back(TypePtr(new Undefined()));
    size++;
  }

  const TypePtr &item = args.at(1);
  items[index] = item;

  return ResultPtr(new Result(context, item));
}

// ----------------------------------------------------------------------------
ResultPtr ArrayClass::op_lshift(const ContextPtr &context, const GroupedParameterList &params) {
  const TypeList &args = params.only_args();
  
  TypeList &items = values(context);
  for (auto &item : args) {
    items.push_back(item);
  }

  return ResultPtr(new Result(context, context->self()));
}

// ----------------------------------------------------------------------------
ResultPtr ArrayClass::op_add(const ContextPtr &context, const GroupedParameterList &params) {
  const TypeList &args = params.only_args();
  TypeList newArray = values(context);

  for (auto &arg : args) {
    for (auto &item : arg->convertToArray(context)) {
      newArray.push_back(item);
    }
  }

  return createInstance(context, newArray);
}

// ----------------------------------------------------------------------------
ResultPtr ArrayClass::to_s(const ContextPtr &context, const GroupedParameterList&) {
  std::ostringstream os;
  bool first=true;

  os << "[";
  for (auto &item : values(context)) {
    if (first) {
      first = false;
    } else {
      os << ", ";
    }

    os << item->convertToString(context);
  }
  os << "]";

  return StringClass::createInstance(context, os.str());
}

// ----------------------------------------------------------------------------
ResultPtr ArrayClass::join(const ContextPtr &context, const GroupedParameterList &parameter) {
  std::string joinUsing = ",";
  const TypeList &args = parameter.only_args();
  if (args.size()) {
    joinUsing = args.front()->convertToString(context);
  }

  std::ostringstream os;
  bool first=true;

  for (auto &item : values(context)) {
    if (first) {
      first = false;
    } else {
      os << joinUsing;
    }

    os << item->convertToString(context);
  }

  return StringClass::createInstance(context, os.str());
}

// ----------------------------------------------------------------------------
InternalArrayValue *ArrayClass::internalArray(const interpret::ContextPtr &context) {
  return internalArray(context, context->self());
}

// ----------------------------------------------------------------------------
InternalArrayValue *ArrayClass::internalArray(const interpret::ContextPtr &context, const TypePtr &self) {
  if (!self->isObjectOfType("Array")) {
    return NULL;
  }

  TypePtr valueObject = self->getMember("__value");
  if (!valueObject->isInternalObjectOfType("Array")) {
    context->log().error() << self->asString() << " is not not an array";
    return NULL;
  }

  return dynamic_cast<InternalArrayValue*>(valueObject.get());
}

// ----------------------------------------------------------------------------
std::vector<types::TypePtr> &ArrayClass::values(const interpret::ContextPtr &context) {
  InternalArrayValue *array = internalArray(context);
  if (array == NULL) {
    return __emptyList;
  }

  return array->getValue();
}

// ----------------------------------------------------------------------------
void ArrayClass::serializeObject(const TypePtr &type, const serialize::SerializeObjectPtr &object) {
  uint32_t numItems = 0;

  InternalArrayValue *array = internalArray(object->getContext(), type);
  if (array == NULL) {
    object->write(&numItems, sizeof(numItems));
    return;
  }

  const auto &values = array->getValue();
  numItems = values.size();
  object->write(&numItems, sizeof(numItems));

  for (const auto &item : values) {
    object->write(item);
  }
}

// ----------------------------------------------------------------------------
bool ArrayClass::deserializeObject(const serialize::DeserializeObjectPtr &object) {
  uint32_t numItems = 0;
  
  TypePtr result = createInstance(object->getContext(), TypeList())->get();
  TypeList &list = internalArray(object->getContext(), result)->getValue();
  
  object->store(result);
  object->read(&numItems, sizeof(numItems));

  for (uint32_t i=0; i<numItems; i++) {
    list.push_back(object->readObject());
  }

  return true;
}
