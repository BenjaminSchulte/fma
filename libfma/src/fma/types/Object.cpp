#include <fma/types/Object.hpp>
#include <fma/types/Class.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/InstanceContext.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/serialize/SerializeObject.hpp>
#include <fma/serialize/SerializerRegistry.hpp>
#include <fma/Project.hpp>
#include <sstream>
#include <iostream>

using namespace FMA;
using namespace FMA::types;
using namespace FMA::interpret;
using namespace FMA::serialize;

// ----------------------------------------------------------------------------
Object::Object(const ClassPtr &klass)
  : klass(klass)
{
}

// ----------------------------------------------------------------------------
ResultPtr Object::callWithoutDecoratorTest(const ContextPtr &context, const GroupedParameterList &params) {
  TypePtr callback = klass->getPrototypeMember("()");
  if (callback->isUndefined()) {
    return Result::executed(context, getPointer());
  }

  ContextPtr callContext(new InstanceContext(context->getInterpreter(), std::dynamic_pointer_cast<Object>(getPointer()), ""));

  return callback->call(callContext, params);
}

// ----------------------------------------------------------------------------
bool Object::isObjectOfType(const std::string &name) const {
  return klass->isInstanceOf(name);
}

// ----------------------------------------------------------------------------
bool Object::hasMember(const std::string &name) const {
  if (Base::hasMember(name)) {
    return true;
  }

  return klass->hasPrototypeMember(name);
}

// ----------------------------------------------------------------------------
TypePtr Object::getMember(const std::string &name) const {
  TypePtr result = Base::getMember(name);
  if (!result->isUndefined()) {
    return result;
  }

  return klass->getPrototypeMember(name);
}

// ----------------------------------------------------------------------------
void Object::dump(const std::string &prefix) {
  std::cout << prefix << "object type:" << klass->getName() << std::endl;
  for (auto &member : members) {
    std::cout << prefix << "  " << member.first << " = " << member.second->asString() << std::endl;
  }
  std::cout << prefix << "end" << std::endl;
}

// ----------------------------------------------------------------------------
std::string Object::asString() const {
  std::ostringstream os;
  os << "<:" << klass->getName() << " @ " << std::hex << this << ">";
  return os.str();
}

// ----------------------------------------------------------------------------
SerializeType Object::getSerializeTypeId() const {
  if (!getClass()->hasMember("@FMA_TYPE_ID")) {
    return TYPE_OBJECT;
  }

  const TypePtr &type = getClass()->getMember("@FMA_TYPE_ID");
  if (!type->isInternalObjectOfType("Number")) {
    return TYPE_OBJECT;
  }

  InternalNumberValue *value = dynamic_cast<InternalNumberValue*>(type.get());
  return (SerializeType)value->getValue();
}

// ----------------------------------------------------------------------------
void Object::serialize(const SerializeObjectPtr &object) {
  SerializeType id = getSerializeTypeId();

  const auto &project = object->getContext()->getProject();
  
  if (!project->serializer()->serialize(id, getPointer(), object)) {
    object->getContext()->log().error() << "Unable to serialize object of class " << getClass()->getName();
  }
}
// ----------------------------------------------------------------------------

