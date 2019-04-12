#include <fma/serialize/SerializerRegistry.hpp>
#include <fma/serialize/SerializeOutput.hpp>
#include <fma/types/Base.hpp>
#include <fma/types/Undefined.hpp>
#include <iostream>

using namespace FMA;
using namespace FMA::types;
using namespace FMA::serialize;

// ----------------------------------------------------------------------------
types::TypePtr SerializerRegistry::deserialize(const DeserializeObjectPtr &object) {
  TypeDeserializerCallbackMap::iterator it = typeDeserializers.find(object->getTypeId());
  
  if (it == typeDeserializers.end()) {
    object->getContext()->log().error() << "Could not find deserializer for object ID " << (int)object->getTypeId();
    return undefinedValue;
  }

  const DeserializerCallbackPtr &callback = it->second;
  if (!callback(object)) {
    object->getContext()->log().error() << "Could not deserialize object";
    return undefinedValue;
  }

  return object->getObject();
}

// ----------------------------------------------------------------------------
bool SerializerRegistry::serialize(SerializeType type, const types::TypePtr &self, const SerializeObjectPtr &object) {
  TypeSerializerCallbackMap::iterator it = typeSerializers.find(type);
  
  if (it == typeSerializers.end()) {
    return false;
  }

  const SerializerCallbackPtr &callback = it->second;
  callback(self, object);
  return true;
}

// ----------------------------------------------------------------------------

