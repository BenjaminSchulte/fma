#include <fma/core/Class.hpp>
#include <fma/core/SerializerBuffer.hpp>
#include <fma/core/Number.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/types/Object.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/Parameter.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/serialize/SerializeOutput.hpp>
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

WeakSerializeObjectPtr emptySerializePointer;

// ----------------------------------------------------------------------------
ClassPtr SerializerBufferClass::create(const RootModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("Buffer$Serializer", "Buffer$Serializer"));
  klass->extends(ClassObject);

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", SerializerBufferClass::initialize)));
  proto->setMember("db", TypePtr(new InternalFunctionValue("db", SerializerBufferClass::db)));
  proto->setMember("dw", TypePtr(new InternalFunctionValue("dw", SerializerBufferClass::dw)));
  proto->setMember("dd", TypePtr(new InternalFunctionValue("dd", SerializerBufferClass::dd)));
  proto->setMember("object", TypePtr(new InternalFunctionValue("object", SerializerBufferClass::object)));

  root->setMember("Buffer$Serializer", klass);
  return klass;
}

// ----------------------------------------------------------------------------
ResultPtr SerializerBufferClass::createInstance(const ContextPtr &context, const serialize::WeakSerializeObjectPtr &object) {
  // Resolve a 
  ClassPtr number = context->getRootLevelContext()->resolve("Buffer$Serializer")->asClass();
  if (!number) {
    return ResultPtr(new Result());
  }

  GroupedParameterList parameters;
  parameters.push_back(TypePtr(new InternalSerializeObjectValue(object)));
  return Result::executed(context, number->createInstance(context, parameters));
}

// ----------------------------------------------------------------------------
ResultPtr SerializerBufferClass::initialize(const ContextPtr &context, const GroupedParameterList &parameters) {
  const TypeList &args = parameters.only_args();
  
  if (args.size()) {
    context->self()->setMember("__buffer", args.front());
  }

  return Result::executed(context, context->self());
}

// ----------------------------------------------------------------------------
ResultPtr SerializerBufferClass::db(const ContextPtr &context, const GroupedParameterList &parameter) {
  return _writeNumber(context, 1, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr SerializerBufferClass::dw(const ContextPtr &context, const GroupedParameterList &parameter) {
  return _writeNumber(context, 2, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr SerializerBufferClass::dd(const ContextPtr &context, const GroupedParameterList &parameter) {
  return _writeNumber(context, 3, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr SerializerBufferClass::object(const ContextPtr &context, const GroupedParameterList &parameter) {
  const WeakSerializeObjectPtr &bufferPtr(getBuffer(context));
  const SerializeObjectPtr &buffer(bufferPtr.lock());
  if (!buffer) {
    return Result::executed(context, context->self());
  }

  const TypeList &args = parameter.only_args();
  
  for (const auto &arg : args) {
    buffer->write(arg);
  }

  return Result::executed(context, context->self());
}


// ----------------------------------------------------------------------------
ResultPtr SerializerBufferClass::_writeNumber(const ContextPtr &context, uint8_t numBytes, const GroupedParameterList &parameter) {
  const WeakSerializeObjectPtr &bufferPtr(getBuffer(context));
  const SerializeObjectPtr &buffer(bufferPtr.lock());
  if (!buffer) {
    return Result::executed(context, context->self());
  }

  const TypeList &args = parameter.only_args();
  uint64_t value;
  
  for (const auto &arg : args) {
    value = arg->convertToNumber(context);
    buffer->write(&value, numBytes);
  }

  return Result::executed(context, context->self());
}

// ----------------------------------------------------------------------------
const WeakSerializeObjectPtr &SerializerBufferClass::getBuffer(const ContextPtr &context) {
  return getBuffer(context->getProject(), context->self());
}

// ----------------------------------------------------------------------------
const WeakSerializeObjectPtr &SerializerBufferClass::getBuffer(Project *project, const TypePtr &value) {
  const TypePtr &obj = value->getMember("__buffer");
  if (!obj || !obj->isInternalObjectOfType("SerializeObject")) {
    project->log().error() << "Unable to access buffer";
    return emptySerializePointer;
  }

  return std::dynamic_pointer_cast<InternalSerializeObjectValue>(obj)->getValue();
}
