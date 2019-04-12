#include <fma/core/Nil.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/serialize/SerializeOutput.hpp>
#include <fma/serialize/SerializeObject.hpp>
#include <fma/serialize/SerializeHeader.hpp>
#include <iostream>

using namespace FMA;
using namespace FMA::output;
using namespace FMA::interpret;
using namespace FMA::serialize;
using namespace FMA::types;

// ----------------------------------------------------------------------------
SerializeOutput::SerializeOutput(const ContextPtr &context)
  : data(new DynamicBuffer())
  , context(context)
{
  root = add(core::NilClass::createInstance(context)->get());
}

// ----------------------------------------------------------------------------
SerializeOutput::~SerializeOutput() {
}

// ----------------------------------------------------------------------------
bool SerializeOutput::containsObject(AnyObjectPtr object) const {
  SerializeObjectMap::const_iterator it = objects.find(object);
  
  return it != objects.end();
}

// ----------------------------------------------------------------------------
SerializeObjectId SerializeOutput::getObjectId(AnyObjectPtr object) const {
  SerializeObjectMap::const_iterator it = objects.find(object);
  
  if (it == objects.end()) {
    return 0;
  } else {
    return it->second;
  }
}

// ----------------------------------------------------------------------------
SerializeStringId SerializeOutput::getString(const std::string &value) {
  SerializeStringMap::const_iterator it = strings.find(value);
  if (it != strings.end()) {
    return it->second;
  }

  SerializeTypeId id = orderedStrings.size();
  orderedStrings.push_back(value);
  strings.insert(std::make_pair(value, id));
  return id;
}

// ----------------------------------------------------------------------------
SerializeObjectPtr SerializeOutput::startObject(SerializeTypeId type, AnyObjectPtr object) {
  SerializeObjectId id = objects.size();
  objects.insert(std::make_pair(object, id));
  return SerializeObjectPtr(new SerializeObject(this, type, id));
}

// ----------------------------------------------------------------------------
void SerializeOutput::endObject(const SerializeObjectPtr &object) {
  offsets.insert(std::make_pair(object->getObjectId(), data->getSize()));
  
  while (orderedOffsets.size() <= object->getObjectId()) {
    orderedOffsets.push_back(0);
  }
  orderedOffsets[object->getObjectId()] = data->getSize();

  SerializeTypeId typeId = object->getTypeId();
  data->write(&typeId, sizeof(SerializeTypeId));
  data->write(object->buf());
}

// ----------------------------------------------------------------------------
SerializeObjectId SerializeOutput::add(const types::TypePtr &type) {
  AnyObjectPtr object(type.get());
  SerializeObjectMap::const_iterator it = objects.find(object);
  
  if (it != objects.end()) {
    return it->second;
  }

  SerializeObjectPtr obj = startObject(type->getSerializeTypeId(), object);
  type->serialize(obj);
  endObject(obj);

  return obj->getObjectId();
}

// ----------------------------------------------------------------------------
DynamicBufferPtr SerializeOutput::generate() {
  SerializeHeader header;
  header.root = root;

  DynamicBufferPtr buffer(new DynamicBuffer());
  buffer->write(&header, sizeof(header));

  // String map
  header.stringMapOffset = buffer->tell();
  SerializeStringId numStrings = orderedStrings.size();
  buffer->write(&numStrings, sizeof(numStrings));
  for (const auto &string : orderedStrings) {
    uint16_t size = string.size();
    buffer->write(&size, 2);
  }
  for (const auto &string : orderedStrings) {
    uint16_t size = string.size();
    buffer->write(string.c_str(), size);
  }

  // Object map
  header.objectMapOffset = buffer->tell();
  SerializeObjectId numOffsets = orderedOffsets.size();
  buffer->write(&numOffsets, sizeof(numOffsets));
  for (const auto &offset : orderedOffsets) {
    SerializeObjectOffset offsetValue = offset;
    buffer->write(&offsetValue, sizeof(offsetValue));
  }

  // Object data
  header.objectDataOffset = buffer->tell();
  buffer->write(data);

  buffer->seek(0);
  buffer->write(&header, sizeof(header));

  return buffer;
}
