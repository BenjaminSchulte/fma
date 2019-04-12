#include <fma/serialize/DeserializeObject.hpp>
#include <fma/serialize/DeserializeInput.hpp>
#include <fma/types/Base.hpp>
#include <fma/types/Undefined.hpp>
#include <iostream>

using namespace FMA;
using namespace FMA::output;
using namespace FMA::types;
using namespace FMA::serialize;
using namespace FMA::interpret;

// ----------------------------------------------------------------------------
DeserializeObject::DeserializeObject(DeserializeInput *input, SerializeType type, SerializeObjectId id, const DynamicBufferAddress &buffer)
  : buffer(buffer)
  , input(input)
  , typeId(type)
  , objectId(id)
  , canReadWithoutObject(false)
{
}

// ----------------------------------------------------------------------------
DeserializeObject::~DeserializeObject() {
}

// ----------------------------------------------------------------------------
const ContextPtr &DeserializeObject::getContext() const {
  return input->getContext();
}

// ----------------------------------------------------------------------------
const types::TypePtr &DeserializeObject::getObject() const {
  return input->getObject(objectId);
}

// ----------------------------------------------------------------------------
void DeserializeObject::store(const TypePtr &value) {
  input->setObject(objectId, value);
}

// ----------------------------------------------------------------------------
uint32_t DeserializeObject::read(void *data, uint32_t size) {
  return buffer.read(data, size);
}

// ----------------------------------------------------------------------------
std::string DeserializeObject::readString() {
  SerializeStringId id;
  if (buffer.read(&id, sizeof(id)) != sizeof(id)) {
    return "";
  }

  return input->getString(id);
}

// ----------------------------------------------------------------------------
TypePtr DeserializeObject::readObject() {
  SerializeObjectId id;
  if (buf().read(&id, sizeof(id)) != sizeof(id)) {
    return undefinedValue;
  }

  return input->deserialize(id);
}

// ----------------------------------------------------------------------------
DynamicBufferAddress &DeserializeObject::buf() {
  if (!canReadWithoutObject && !input->hasObject(objectId)) {
    input->getContext()->log().fatal() << "Deserialize::buf() is invalid until object has been stored";
  }

  return buffer;
}

// ----------------------------------------------------------------------------

