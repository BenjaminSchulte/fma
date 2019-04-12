#include <fma/serialize/SerializeObject.hpp>
#include <fma/serialize/SerializeOutput.hpp>
#include <fma/types/Base.hpp>

using namespace FMA;
using namespace FMA::output;
using namespace FMA::types;
using namespace FMA::serialize;
using namespace FMA::interpret;

// ----------------------------------------------------------------------------
SerializeObject::SerializeObject(SerializeOutput *output, SerializeTypeId type, SerializeObjectId obj)
  : buffer(new DynamicBuffer())
  , output(output)
  , typeId(type)
  , objectId(obj)
{
}

// ----------------------------------------------------------------------------
SerializeObject::~SerializeObject() {
}

// ----------------------------------------------------------------------------
void SerializeObject::write(const TypePtr &type) {
  SerializeObjectId id = output->add(type);
  write(&id, sizeof(id));
}

// ----------------------------------------------------------------------------
void SerializeObject::write(const std::string &string) {
  SerializeStringId id = output->getString(string);
  write(&id, sizeof(id));
}

// ----------------------------------------------------------------------------
const ContextPtr &SerializeObject::getContext() const {
  return output->getContext();
}

// ----------------------------------------------------------------------------

