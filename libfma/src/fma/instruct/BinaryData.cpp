#include <fma/instruct/BinaryData.hpp>
#include <sstream>
#include <iostream>
#include <cstring>

using namespace FMA;
using namespace FMA::assem;
using namespace FMA::instruct;

// ----------------------------------------------------------------------------
BinaryData::BinaryData(const void *_data, uint32_t size)
  : size(size)
  , memoryOffset(size)
  , memorySize(size)
{
  data = malloc(size);
  std::memcpy(data, _data, size);
}

// ----------------------------------------------------------------------------
void BinaryData::appendBinaryData(const void *_data, uint32_t _size) {
  if (memoryOffset + _size > memorySize) {
    memorySize = memoryOffset + _size + 1024;
    
    void *newData = malloc(memorySize);
    memcpy(newData, data, memoryOffset);
    free(data);
    data = newData;
  }

  memcpy((char*)data + memoryOffset, _data, _size);
  memoryOffset += _size;
  size += _size;
}

// ----------------------------------------------------------------------------
BinaryData::~BinaryData() {
  free(data);
}

// ----------------------------------------------------------------------------
std::string BinaryData::getName() {
  return "BINDAT";
}

// ----------------------------------------------------------------------------
std::string BinaryData::asString() {
  std::ostringstream os;
  os << "BINDAT <Blob " << size << ">";
  return os.str();
}

// ----------------------------------------------------------------------------
