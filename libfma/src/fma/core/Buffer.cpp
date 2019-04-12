#include <fma/core/Class.hpp>
#include <fma/core/Buffer.hpp>
#include <fma/core/Number.hpp>
#include <fma/output/DynamicBuffer.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/types/Object.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/Parameter.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/serialize/SerializeOutput.hpp>
#include <fma/serialize/SerializeTypes.hpp>
#include <fma/serialize/DeserializeInput.hpp>
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

// ----------------------------------------------------------------------------
ClassPtr BufferClass::create(const RootModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("Buffer", "Buffer"));
  klass->extends(ClassObject);

  klass->setMember("@FMA_TYPE_ID", TypePtr(new InternalNumberValue(serialize::TYPE_BUFFER)));

  klass->setMember("serialize_object", TypePtr(new InternalFunctionValue("serialize_object", BufferClass::serialize_object)));

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", BufferClass::initialize)));
  proto->setMember("size", TypePtr(new InternalFunctionValue("size", BufferClass::size)));
  proto->setMember("length", TypePtr(new InternalFunctionValue("length", BufferClass::size)));
  proto->setMember("deserialize_object", TypePtr(new InternalFunctionValue("deserialize_object", BufferClass::deserialize_object)));

  proto->setMember("db", TypePtr(new InternalFunctionValue("db", BufferClass::db)));
  proto->setMember("dw", TypePtr(new InternalFunctionValue("dw", BufferClass::dw)));
  proto->setMember("dd", TypePtr(new InternalFunctionValue("dd", BufferClass::dd)));

  root->setMember("Buffer", klass);
  return klass;
}

// ----------------------------------------------------------------------------
ResultPtr BufferClass::createInstance(const ContextPtr &context) {
  // Resolve a 
  ClassPtr number = context->getRootLevelContext()->resolve("Buffer")->asClass();
  if (!number) {
    return ResultPtr(new Result());
  }

  GroupedParameterList parameters;
  return Result::executed(context, number->createInstance(context, parameters));
}

// ----------------------------------------------------------------------------
ResultPtr BufferClass::serialize_object(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    return ResultPtr(new Result());
  }

  SerializeOutput output(context);
  output.setRootObject(output.add(args.front()));
  DynamicBufferPtr buffer = output.generate();

  TypePtr item = createInstance(context)->get();
  item->setMember("__buffer", TypePtr(new InternalBufferValue(buffer)));
  return Result::executed(context, item);
}

// ----------------------------------------------------------------------------
ResultPtr BufferClass::deserialize_object(const ContextPtr &context, const GroupedParameterList &) {
  DeserializeInput input(context, getBuffer(context));
  if (!input.readHeader()) {
    context->log().error() << "Could not read header. Serialized data might be invalid for this version";
    return ResultPtr(new Result());
  }

  TypePtr result = input.deserialize();

  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
ResultPtr BufferClass::initialize(const ContextPtr &context, const GroupedParameterList &) {
  DynamicBufferPtr buffer(new DynamicBuffer());
  context->self()->setMember("__buffer", TypePtr(new InternalBufferValue(buffer)));

  return Result::executed(context, context->self());
}

// ----------------------------------------------------------------------------
ResultPtr BufferClass::size(const ContextPtr &context, const GroupedParameterList &) {
  const DynamicBufferPtr &buffer(getBuffer(context));
  if (!buffer) {
    return ResultPtr(new Result());
  }

  return NumberClass::createInstance(context, buffer->getSize());
}

// ----------------------------------------------------------------------------
ResultPtr BufferClass::db(const ContextPtr &context, const GroupedParameterList &parameter) {
  return _writeNumber(context, 1, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr BufferClass::dw(const ContextPtr &context, const GroupedParameterList &parameter) {
  return _writeNumber(context, 2, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr BufferClass::dd(const ContextPtr &context, const GroupedParameterList &parameter) {
  return _writeNumber(context, 3, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr BufferClass::_writeNumber(const ContextPtr &context, uint8_t numBytes, const GroupedParameterList &parameter) {
  const DynamicBufferPtr &buffer(getBuffer(context));
  const TypeList &args = parameter.only_args();
  uint64_t value;
  
  for (const auto &arg : args) {
    value = arg->convertToNumber(context);
    buffer->write(&value, numBytes);
  }

  return Result::executed(context, context->self());
}

// ----------------------------------------------------------------------------
void BufferClass::serializeObject(const TypePtr &type, const serialize::SerializeObjectPtr &object) {
  const DynamicBufferPtr &buffer(getBuffer(object->getContext()->getProject(), type));

  uint32_t size = buffer->getSize();
  object->write(&size, sizeof(size));
  object->write(buffer->getData(), size);
}

// ----------------------------------------------------------------------------
bool BufferClass::deserializeObject(const serialize::DeserializeObjectPtr &object) {
  object->allowBufferReadWithoutObject();

  uint32_t size;
  if (object->read(&size, sizeof(size) != sizeof(size))) {
    return false;
  }

  void *data = malloc(size);
  if (object->read(data, size) < size) {
    free(data);
    return false;
  }

  DynamicBufferPtr buffer(new DynamicBuffer(data, size));

  TypePtr item = createInstance(object->getContext())->get();
  item->setMember("__buffer", TypePtr(new InternalBufferValue(buffer)));
  object->store(item);

  return true;
}

// ----------------------------------------------------------------------------
DynamicBufferPtr BufferClass::getBuffer(const ContextPtr &context) {
  return getBuffer(context->getProject(), context->self());
}

// ----------------------------------------------------------------------------
DynamicBufferPtr BufferClass::getBuffer(Project *project, const TypePtr &value) {
  const TypePtr &obj = value->getMember("__buffer");
  if (!obj || !obj->isInternalObjectOfType("DynamicBuffer")) {
    project->log().error() << "Unable to access buffer";
    return NULL;
  }

  return std::dynamic_pointer_cast<InternalBufferValue>(obj)->getValue();
}
