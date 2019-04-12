#ifndef __FMA_SERIALIZE_DESERIALIZEOBJECT_H__
#define __FMA_SERIALIZE_DESERIALIZEOBJECT_H__

#include "../output/DynamicBuffer.hpp"
#include "SerializeTypes.hpp"

namespace FMA {
namespace interpret {
  typedef std::shared_ptr<class BaseContext> ContextPtr;
}
namespace types {
  typedef std::shared_ptr<class Base> TypePtr;
}
namespace serialize {

class DeserializeInput;
class DeserializeObject;

typedef std::shared_ptr<DeserializeObject> DeserializeObjectPtr;
typedef std::weak_ptr<DeserializeObject> WeakDeserializeObjectPtr;

class DeserializeObject {
public:
  DeserializeObject(DeserializeInput *input, SerializeType type, SerializeObjectId id, const output::DynamicBufferAddress &buffer);
  virtual ~DeserializeObject();

  void store(const types::TypePtr &type);
  inline void allowBufferReadWithoutObject() {
    canReadWithoutObject = true;
  }

  inline SerializeType getTypeId() const { return typeId; }
  inline SerializeObjectId getObjectId() const { return objectId; }
  const types::TypePtr &getObject() const;
  output::DynamicBufferAddress &buf();
  const interpret::ContextPtr &getContext() const;

  uint32_t read(void *data, uint32_t size);
  std::string readString();
  types::TypePtr readObject();

protected:
  output::DynamicBufferAddress buffer;
  DeserializeInput *input;
  SerializeType typeId;
  SerializeObjectId objectId;
  bool canReadWithoutObject;
};

}
}

#endif