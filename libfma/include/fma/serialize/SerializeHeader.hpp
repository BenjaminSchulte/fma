#ifndef __FMA_SERIALIZE_SERIALIZEHEADER_H__
#define __FMA_SERIALIZE_SERIALIZEHEADER_H__

#include "SerializeTypes.hpp"

namespace FMA {
namespace serialize {

struct __attribute__((packed, aligned(1))) SerializeHeader {
  SerializeHeader() : identifier(*(reinterpret_cast<const uint32_t*>("FMAO"))), version(1) {
  }

  inline bool isValid() const {
    return identifier == *(reinterpret_cast<const uint32_t*>("FMAO")) && version == 1;
  }

  uint32_t identifier;
  uint16_t version;

  SerializeObjectId root;

  uint32_t objectMapOffset;
  uint32_t stringMapOffset;

  uint32_t objectDataOffset;
};

}
}

#endif