#include <fma/core/Nil.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/serialize/DeserializeInput.hpp>
#include <fma/serialize/SerializeHeader.hpp>
#include <fma/serialize/DeserializeObject.hpp>
#include <fma/serialize/SerializerRegistry.hpp>
#include <fma/types/Undefined.hpp>
#include <fma/Project.hpp>
#include <iostream>

using namespace FMA;
using namespace FMA::output;
using namespace FMA::interpret;
using namespace FMA::serialize;
using namespace FMA::types;

// ----------------------------------------------------------------------------
DeserializeInput::DeserializeInput(const ContextPtr &context, const DynamicBufferPtr &buffer)
  : data(buffer)
  , context(context)
  , stringOffsets(NULL)
{
}

// ----------------------------------------------------------------------------
DeserializeInput::~DeserializeInput() {
  delete[] stringOffsets;
}

// ----------------------------------------------------------------------------
bool DeserializeInput::readHeader() {
  data->rewind();
  
  if (data->read(&header, sizeof(header)) < sizeof(header)) {
    return false;
  }

  if (!header.isValid()) {
    return false;
  }

  // Read string map
  data->seek(header.stringMapOffset);
  data->read(&numStrings, sizeof(numStrings));
  stringOffsets = new uint32_t[numStrings];
  uint32_t offset = header.stringMapOffset + sizeof(SerializeStringId) + numStrings * 2; 
  uint16_t stringSize;
  for (SerializeStringId i=0; i<numStrings; i++) {
    data->read(&stringSize, sizeof(stringSize));

    stringOffsets[i] = offset;
    offset += stringSize;
  }

  return true;
}

// ----------------------------------------------------------------------------
std::string DeserializeInput::getString(SerializeStringId id) const {
  uint16_t length;
  uint32_t offset = header.stringMapOffset + sizeof(SerializeStringId) + id * sizeof(length);

  if (data->read(offset, &length, sizeof(length)) != sizeof(length)) {
    return "<Invalid string>";
  };

  if (id >= numStrings) {
    return "<Invalid string>";
  }

  char *buffer = (char*)malloc(length);
  if (data->read(stringOffsets[id], buffer, length) < length) {
    return "<Invalid string>";
  }

  std::string str(buffer, length);
  return str;
}

// ----------------------------------------------------------------------------
bool DeserializeInput::hasObject(SerializeObjectId id) {
  return objects.find(id) != objects.end();
}

// ----------------------------------------------------------------------------
const types::TypePtr &DeserializeInput::getObject(SerializeObjectId id) {
  DeserializeObjectMap::iterator it = objects.find(id);
  if (it != objects.end()) {
    return it->second;
  } 

  return undefinedValue;
}

// ----------------------------------------------------------------------------
void DeserializeInput::setObject(SerializeObjectId id, const TypePtr &value) {
  objects[id] = value;
}

// ----------------------------------------------------------------------------
types::TypePtr DeserializeInput::deserialize() {
  return deserialize(header.root);
}

// ----------------------------------------------------------------------------
types::TypePtr DeserializeInput::deserialize(SerializeObjectId id) {
  DeserializeObjectMap::iterator it = objects.find(id);
  if (it != objects.end()) {
    return it->second;
  }

  SerializeObjectOffset offset;
  SerializeTypeId typeId;

  data->read(header.objectMapOffset + sizeof(SerializeObjectId) + id * sizeof(SerializeObjectOffset), &offset, sizeof(SerializeObjectOffset));
  offset += header.objectDataOffset;

  DynamicBufferAddress buffer(data, offset);
  if (!buffer.read(&typeId, sizeof(SerializeTypeId))) {
    return undefinedValue;
  }

  SerializeType type = (SerializeType)typeId;
  DeserializeObjectPtr object(new DeserializeObject(this, type, id, buffer));
  
  return context->getProject()->serializer()->deserialize(object);
}