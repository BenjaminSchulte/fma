#ifndef __FMA_SERIALIZE_SERIALIZEINPUT_H__
#define __FMA_SERIALIZE_SERIALIZEINPUT_H__

#include <map>
#include "SerializeTypes.hpp"
#include "SerializeHeader.hpp"
#include "../output/DynamicBuffer.hpp"
#include "../interpret/BaseContext.hpp"

namespace FMA {
namespace types {
  typedef std::shared_ptr<class Base> TypePtr;
}
namespace serialize {

typedef std::map<SerializeObjectId, types::TypePtr> DeserializeObjectMap;

class DeserializeInput {
public:
  DeserializeInput(const interpret::ContextPtr &context, const output::DynamicBufferPtr &buffer);
  virtual ~DeserializeInput();

  bool readHeader();

  bool hasObject(SerializeObjectId id);
  void setObject(SerializeObjectId id, const types::TypePtr &value);
  const types::TypePtr &getObject(SerializeObjectId id);

  types::TypePtr deserialize();
  types::TypePtr deserialize(SerializeObjectId id);

  std::string getString(SerializeStringId id) const;

  inline const interpret::ContextPtr &getContext() const {
    return context;
  }

protected:
  output::DynamicBufferPtr data;
  interpret::ContextPtr context;
  SerializeHeader header;

  DeserializeObjectMap objects;

  SerializeStringId numStrings;
  uint32_t *stringOffsets;
};

}
}

#endif