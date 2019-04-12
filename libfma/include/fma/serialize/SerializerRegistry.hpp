#ifndef __FMA_SERIALIZE_SERIALIZERREGISTRY_H__
#define __FMA_SERIALIZE_SERIALIZERREGISTRY_H__

#include <map>
#include <functional>
#include "SerializeObject.hpp"
#include "DeserializeObject.hpp"

namespace FMA {
namespace types {
typedef std::shared_ptr<class Base> TypePtr;
typedef std::weak_ptr<class Class> WeakClassPtr;
}
namespace serialize {

typedef std::function<void(const types::TypePtr &, const SerializeObjectPtr &)> SerializerCallbackPtr;
typedef std::function<bool(const DeserializeObjectPtr &object)> DeserializerCallbackPtr;

typedef std::map<SerializeType, SerializerCallbackPtr> TypeSerializerCallbackMap;
typedef std::map<SerializeType, DeserializerCallbackPtr> TypeDeserializerCallbackMap;
typedef std::map<std::string, types::WeakClassPtr> TypeDeserializerClassMap;

class SerializerRegistry {
public:

  inline void registerSerializer(SerializeType type, const SerializerCallbackPtr &callback) {
    typeSerializers[type] = callback;
  }

  inline void registerDeserializer(SerializeType type, const DeserializerCallbackPtr &callback) {
    typeDeserializers[type] = callback;
  }

  inline void registerClass(const std::string &name, const types::WeakClassPtr &klass) {
    classes[name] = klass;
  }
  
  bool serialize(SerializeType type, const types::TypePtr &self, const SerializeObjectPtr &object);
  types::TypePtr deserialize(const DeserializeObjectPtr &object);

  inline const TypeDeserializerClassMap &allClasses() const { return classes; };

protected:
  TypeSerializerCallbackMap typeSerializers;
  TypeDeserializerCallbackMap typeDeserializers;
  TypeDeserializerClassMap classes;
};

}
}

#endif