#ifndef __FMA_SERIALIZE_SERIALIZEOUTPUT_H__
#define __FMA_SERIALIZE_SERIALIZEOUTPUT_H__

#include <map>
#include <vector>
#include "SerializeTypes.hpp"
#include "../output/DynamicBuffer.hpp"
#include "../interpret/BaseContext.hpp"

namespace FMA {
namespace types {
  typedef std::shared_ptr<class Base> TypePtr;
}
namespace serialize {

class SerializeObject;

typedef std::map<AnyObjectPtr, SerializeObjectId> SerializeObjectMap;
typedef std::map<SerializeObjectId, SerializeObjectOffset> SerializeObjectOffsetMap;
typedef std::map<std::string, SerializeStringId> SerializeStringMap;

typedef std::vector<std::string> SerializeStringList;
typedef std::vector<SerializeObjectOffset> SerializeObjectOffsetList;

class SerializeOutput {
public:
  SerializeOutput(const interpret::ContextPtr &context);
  virtual ~SerializeOutput();

  bool containsObject(AnyObjectPtr object) const;
  SerializeObjectId getObjectId(AnyObjectPtr object) const;
  SerializeStringId getString(const std::string &value);

  SerializeObjectPtr startObject(SerializeTypeId type, AnyObjectPtr object);
  void endObject(const SerializeObjectPtr &object);

  SerializeObjectId add(const types::TypePtr &type);
  inline void setRootObject(SerializeObjectId id) {
    root = id;
  }

  output::DynamicBufferPtr generate();
  
  inline const interpret::ContextPtr &getContext() const {
    return context;
  }

protected:
  SerializeObjectMap objects;
  SerializeObjectOffsetMap offsets;
  SerializeObjectOffsetList orderedOffsets;
  SerializeStringMap strings;
  SerializeStringList orderedStrings;
  SerializeObjectId root;
  output::DynamicBufferPtr data;
  interpret::ContextPtr context;
};

}
}

#endif