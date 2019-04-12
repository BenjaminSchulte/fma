#include <fma/core/Class.hpp>
#include <fma/core/DeserializerBuffer.hpp>
#include <fma/core/Number.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/types/Object.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/Parameter.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/serialize/DeserializeInput.hpp>
#include <fma/serialize/SerializeTypes.hpp>
#include <fma/Project.hpp>

using namespace FMA;
using namespace FMA::core;
using namespace FMA::types;
using namespace FMA::interpret;
using namespace FMA::serialize;
using namespace FMA::output;

#include <iostream>
#include <fstream>
#include <sstream>

WeakDeserializeObjectPtr emptyDeserializePtr;

// ----------------------------------------------------------------------------
ClassPtr DeserializerBufferClass::create(const RootModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("Buffer$Deserializer", "Buffer$Deserializer"));
  klass->extends(ClassObject);

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", DeserializerBufferClass::initialize)));
  proto->setMember("store", TypePtr(new InternalFunctionValue("store", DeserializerBufferClass::store)));
  proto->setMember("db", TypePtr(new InternalFunctionValue("db", DeserializerBufferClass::db)));
  proto->setMember("dw", TypePtr(new InternalFunctionValue("dw", DeserializerBufferClass::dw)));
  proto->setMember("dd", TypePtr(new InternalFunctionValue("dd", DeserializerBufferClass::dd)));
  proto->setMember("object", TypePtr(new InternalFunctionValue("object", DeserializerBufferClass::object)));

  root->setMember("Buffer$Deserializer", klass);
  return klass;
}

// ----------------------------------------------------------------------------
ResultPtr DeserializerBufferClass::createInstance(const ContextPtr &context, const serialize::WeakDeserializeObjectPtr &object) {
  // Resolve a 
  ClassPtr number = context->getRootLevelContext()->resolve("Buffer$Deserializer")->asClass();
  if (!number) {
    return ResultPtr(new Result());
  }

  GroupedParameterList parameters;
  parameters.push_back(TypePtr(new InternalDeserializeObjectValue(object)));
  return Result::executed(context, number->createInstance(context, parameters));
}

// ----------------------------------------------------------------------------
ResultPtr DeserializerBufferClass::initialize(const ContextPtr &context, const GroupedParameterList &parameters) {
  const TypeList &args = parameters.only_args();
  
  if (args.size()) {
    context->self()->setMember("__buffer", args.front());
  }

  return Result::executed(context, context->self());
}

// ----------------------------------------------------------------------------
ResultPtr DeserializerBufferClass::store(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    return ResultPtr(new Result());
  }

  const WeakDeserializeObjectPtr &bufferPtr(getBuffer(context));
  const DeserializeObjectPtr &buffer(bufferPtr.lock());
  if (!buffer) {
    return Result::executed(context, context->self());
  }

  buffer->store(args.front());

  return Result::executed(context, args.front());
}

// ----------------------------------------------------------------------------
ResultPtr DeserializerBufferClass::db(const ContextPtr &context, const GroupedParameterList &parameter) {
  return _readNumber(context, 1, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr DeserializerBufferClass::dw(const ContextPtr &context, const GroupedParameterList &parameter) {
  return _readNumber(context, 2, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr DeserializerBufferClass::dd(const ContextPtr &context, const GroupedParameterList &parameter) {
  return _readNumber(context, 3, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr DeserializerBufferClass::object(const ContextPtr &context, const GroupedParameterList &) {
  const WeakDeserializeObjectPtr &bufferPtr(getBuffer(context));
  const DeserializeObjectPtr &buffer(bufferPtr.lock());
  if (!buffer) {
    return Result::executed(context, context->self());
  }

  return Result::executed(context, buffer->readObject());
}


// ----------------------------------------------------------------------------
ResultPtr DeserializerBufferClass::_readNumber(const ContextPtr &context, uint8_t numBytes, const GroupedParameterList &) {
  const WeakDeserializeObjectPtr &bufferPtr(getBuffer(context));
  const DeserializeObjectPtr &buffer(bufferPtr.lock());
  if (!buffer) {
    return Result::executed(context, context->self());
  }

  uint64_t value;
  buffer->read(&value, numBytes);

  return NumberClass::createInstance(context, value);
}

// ----------------------------------------------------------------------------
const WeakDeserializeObjectPtr &DeserializerBufferClass::getBuffer(const ContextPtr &context) {
  return getBuffer(context->getProject(), context->self());
}

// ----------------------------------------------------------------------------
const WeakDeserializeObjectPtr &DeserializerBufferClass::getBuffer(Project *project, const TypePtr &value) {
  const TypePtr &obj = value->getMember("__buffer");
  if (!obj || !obj->isInternalObjectOfType("DeserializeObject")) {
    project->log().error() << "Unable to access buffer";
    return emptyDeserializePtr;
  }

  return std::dynamic_pointer_cast<InternalDeserializeObjectValue>(obj)->getValue();
}
