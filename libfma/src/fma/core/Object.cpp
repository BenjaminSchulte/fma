#include <fma/core/Object.hpp>
#include <fma/core/Boolean.hpp>
#include <fma/core/String.hpp>
#include <fma/core/SerializerBuffer.hpp>
#include <fma/core/DeserializerBuffer.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/types/Object.hpp>
#include <fma/types/Class.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/serialize/SerializeObject.hpp>
#include <fma/serialize/SerializerRegistry.hpp>
#include <fma/Project.hpp>
#include <iostream>

using namespace FMA;
using namespace FMA::core;
using namespace FMA::types;
using namespace FMA::interpret;

// ----------------------------------------------------------------------------
ClassPtr ObjectClass::create(const RootModulePtr &root) {
  ClassPtr klass = ClassPtr(new Class("Object", "Object"));
  klass->setMember("respond_to?", TypePtr(new InternalFunctionValue("respond_to?", ObjectClass::respond_to_class_qm)));
  klass->setMember("has_attribute?", TypePtr(new InternalFunctionValue("has_attribute?", ObjectClass::has_attribute_qm)));

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("__dump__", TypePtr(new InternalFunctionValue("__dump__", ObjectClass::__dump__)));
  proto->setMember("has_attribute?", TypePtr(new InternalFunctionValue("has_attribute?", ObjectClass::has_attribute_qm)));
  proto->setMember("respond_to?", TypePtr(new InternalFunctionValue("respond_to?", ObjectClass::respond_to_qm)));
  proto->setMember("is_a?", TypePtr(new InternalFunctionValue("is_a?", ObjectClass::is_a_qm)));
  proto->setMember("nil?", TypePtr(new InternalFunctionValue("nil?", ObjectClass::nil_qm)));
  proto->setMember("to_s", TypePtr(new InternalFunctionValue("to_s", ObjectClass::to_s)));
  proto->setMember("to_b", TypePtr(new InternalFunctionValue("to_b", ObjectClass::to_b)));
  proto->setMember("class", TypePtr(new InternalFunctionValue("class", ObjectClass::get_class)));

  proto->setMember("==", TypePtr(new InternalFunctionValue("==", ObjectClass::op_eq)));

  root->setMember("Object", klass);
  return klass;
}

// ----------------------------------------------------------------------------
ResultPtr ObjectClass::__dump__(const ContextPtr &context, const GroupedParameterList&) {
  context->self()->dump("");

  return BooleanClass::createInstance(context, false);
}

// ----------------------------------------------------------------------------
ResultPtr ObjectClass::has_attribute_qm(const ContextPtr &context, const GroupedParameterList &params) {
  const TypeList &args = params.only_args();
  if (!args.size()) {
    return BooleanClass::createInstance(context, false);
  }

  return BooleanClass::createInstance(context, context->self()->hasMember(args.front()->convertToString(context)));
}

// ----------------------------------------------------------------------------
ResultPtr ObjectClass::respond_to_qm(const ContextPtr &context, const GroupedParameterList &params) {
  const TypeList &args = params.only_args();
  if (!args.size()) {
    return BooleanClass::createInstance(context, false);
  }

  return BooleanClass::createInstance(context, context->self()->asObject()->getClass()->hasMember(args.front()->convertToString(context)));
}

// ----------------------------------------------------------------------------
ResultPtr ObjectClass::respond_to_class_qm(const ContextPtr &context, const GroupedParameterList &params) {
  const TypeList &args = params.only_args();
  if (!args.size()) {
    return BooleanClass::createInstance(context, false);
  }
  
  return BooleanClass::createInstance(context, context->self()->asClass()->hasMember(args.front()->convertToString(context)));
}

// ----------------------------------------------------------------------------
ResultPtr ObjectClass::is_a_qm(const ContextPtr &context, const GroupedParameterList &params) {
  const TypeList &args = params.only_args();
  if (!args.size()) {
    return BooleanClass::createInstance(context, false);
  }

  ClassPtr klass = args.front()->asClass();
  if (!klass) {
    return BooleanClass::createInstance(context, false);
  }

  ObjectPtr self = context->self()->asObject();
  if (!self) {
    return BooleanClass::createInstance(context, false);
  }

  ClassPtr selfKlass = self->getClass();
  return BooleanClass::createInstance(context, selfKlass->isInstanceOf(klass));
}

// ----------------------------------------------------------------------------
ResultPtr ObjectClass::nil_qm(const ContextPtr &context, const GroupedParameterList&) {
  return BooleanClass::createInstance(context, false);
}

// ----------------------------------------------------------------------------
ResultPtr ObjectClass::to_s(const ContextPtr &context, const GroupedParameterList&) {
  return StringClass::createInstance(context, context->self()->asString());
}

// ----------------------------------------------------------------------------
ResultPtr ObjectClass::to_b(const ContextPtr &context, const GroupedParameterList&) {
  return BooleanClass::createInstance(context, true);
}

// ----------------------------------------------------------------------------
ResultPtr ObjectClass::get_class(const ContextPtr &context, const GroupedParameterList&) {
  ClassPtr klass = context->self()->asObject()->getClass();
  return ResultPtr(new Result(context, klass));
}

// ----------------------------------------------------------------------------
ResultPtr ObjectClass::op_eq(const ContextPtr &context, const GroupedParameterList &params) {
  const TypeList &args = params.only_args();
  if (!args.size()) {
    return BooleanClass::createInstance(context, false);
  }

  return BooleanClass::createInstance(context, args.front().get() == context->self().get());
}

// ----------------------------------------------------------------------------
void ObjectClass::serializeObject(const TypePtr &_self, const serialize::SerializeObjectPtr &object) {
  ObjectPtr self = _self->asObject();
  uint32_t numMembers = self->allMembers().size();
  
  object->write(self->getClass()->getFullName());

  if (self->getClass()->hasPrototypeMember("__serialize")) {
    GroupedParameterList params;
    params.push_back(SerializerBufferClass::createInstance(object->getContext(), object)->get());
    self->callDirect("__serialize", object->getContext(), params);
  } else {
    object->getContext()->getProject()->log().notice() << "Serializing custom object of type " << self->getClass()->getName();

    object->write(self->getClass()->getFullName());
    object->write(&numMembers, sizeof(numMembers));

    for (const auto &member : self->allMembers()) {
      object->write(member.first);
      object->write(member.second);
    }
  }
}

// ----------------------------------------------------------------------------
bool ObjectClass::deserializeObject(const serialize::DeserializeObjectPtr &object) {
  std::string className = object->readString();

  const serialize::TypeDeserializerClassMap &klasses = object->getContext()->getProject()->serializer()->allClasses();
  serialize::TypeDeserializerClassMap::const_iterator it = klasses.find(className);

  if (it == klasses.end()) {
    object->getContext()->log().error() << "Unable to find serialized class " << className;
    return false;
  }
  
  const WeakClassPtr &klassPtr = it->second;
  ClassPtr klass = klassPtr.lock();
  if (!klass) {
    object->getContext()->log().error() << "Class " << className << " is already deconstructed and can not longed be accessed.";
    return false;
  }

  if (!klass->hasMember("__deserialize")) {
    object->getContext()->log().error() << "Class " << className << " does not provide a __deserialize callback.";
    return false;
  }

  GroupedParameterList params;
  params.push_back(DeserializerBufferClass::createInstance(object->getContext(), object)->get());

  TypePtr result = klass->callDirect("__deserialize", object->getContext(), params)->get();
  if (result->isUndefined()) {
    return false;
  }

  object->store(result);
  return true;
}

// ----------------------------------------------------------------------------