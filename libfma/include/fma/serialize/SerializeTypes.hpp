#ifndef __FMA_SERIALIZE_SERIALIZETYPES_H__
#define __FMA_SERIALIZE_SERIALIZETYPES_H__

#include <map>

namespace FMA {
namespace serialize {

enum SerializeType {
  TYPE_NIL = 0,
  TYPE_ARRAY = 1,
  TYPE_BOOLEAN = 2,
  TYPE_BUFFER = 3,
  TYPE_DATABLOCK = 4,
  TYPE_FUNCTION = 5,
  TYPE_IMMEDIATENUMBER = 6,
  TYPE_MAP = 7,
  TYPE_NUMBER = 8,
  TYPE_RANGE = 9,
  TYPE_STRING = 10,
  TYPE_SYMBOLREFERENCE = 11,
  TYPE_TYPEDNUMBER = 12,
  TYPE_OBJECT = 0x80,
  TYPE_INVALID = 0xFF
};

typedef void* AnyObjectPtr;
typedef uint32_t SerializeObjectId;
typedef uint32_t SerializeObjectOffset;
typedef uint8_t SerializeTypeId;
typedef uint32_t SerializeStringId;

}
}

#endif