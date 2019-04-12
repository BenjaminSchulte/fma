#ifndef __FMA_SERIALIZE_SERIALIZEOBJECT_H__
#define __FMA_SERIALIZE_SERIALIZEOBJECT_H__

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

class SerializeOutput;
class SerializeObject;

typedef std::shared_ptr<SerializeObject> SerializeObjectPtr;
typedef std::weak_ptr<SerializeObject> WeakSerializeObjectPtr;

class SerializeObject {
public:
  SerializeObject(SerializeOutput *output, SerializeTypeId type, SerializeObjectId obj);
  virtual ~SerializeObject();

  inline SerializeTypeId getTypeId() const { return typeId; }
  inline SerializeObjectId getObjectId() const { return objectId; }

  inline const output::DynamicBufferPtr &buf() const { return buffer; }

  const interpret::ContextPtr &getContext() const;

  void write(const std::string &string);

  inline void write(const void *data, uint32_t size) {
    buffer->write(data, size);
  }

  inline void write(const output::DynamicBufferPtr &other) {
    buffer->write(other);
  }

  void write(const types::TypePtr &type);

protected:
  output::DynamicBufferPtr buffer;
  SerializeOutput *output;
  SerializeTypeId typeId;
  SerializeObjectId objectId;
};

}
}

#endif