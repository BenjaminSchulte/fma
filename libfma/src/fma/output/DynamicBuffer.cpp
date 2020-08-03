#include <fma/output/DynamicBuffer.hpp>
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace FMA::output;

// ----------------------------------------------------------------------------
DynamicBuffer::DynamicBuffer() : offset(0), size(0), memorySize(256) {
  data = malloc(memorySize);
  memset(data, 0, memorySize);
}

// ----------------------------------------------------------------------------
DynamicBuffer::DynamicBuffer(void *data, uint32_t size)
  : data(data)
  , offset(0)
  , size(size)
  , memorySize(size)
{
}

// ----------------------------------------------------------------------------
DynamicBuffer::~DynamicBuffer() {
  free(data);
}

// ----------------------------------------------------------------------------
void DynamicBuffer::forceSize(uint32_t size) {
  if (offset >= size) {
    return;
  }

  allocate(size - offset);
}

// ----------------------------------------------------------------------------
void DynamicBuffer::allocate(uint32_t _size) {
  uint32_t newSize = offset + _size;
  if (newSize <= memorySize) {
    size = newSize > size ? newSize : size;
    return;
  }

  memorySize = newSize + 256;

  data = realloc(data, memorySize);
  size = newSize > size ? newSize : size;
}

// ----------------------------------------------------------------------------
void DynamicBuffer::seek(uint32_t _offset) {
  offset = _offset;
}

// ----------------------------------------------------------------------------
void DynamicBuffer::rewind() {
  offset = 0;
}

// ----------------------------------------------------------------------------
uint32_t DynamicBuffer::read(void *_data, uint32_t _size) {
  _size = read(offset, _data, _size);
  offset += _size;
  return _size;
}

// ----------------------------------------------------------------------------
uint32_t DynamicBuffer::read(uint32_t _offset, void *_data, uint32_t _size) {
  _size = std::min(_offset + _size, size) - _offset;
  memcpy(_data, (char*)data + _offset, _size);
  return _size;
}

// ----------------------------------------------------------------------------
void DynamicBuffer::write(const void *_data, uint32_t _size) {
  if (!_size) {
    return;
  }

  allocate(_size);
  memcpy((char*)data + offset, _data, _size);
  offset += _size;
}

// ----------------------------------------------------------------------------
void DynamicBuffer::write(const DynamicBufferPtr &buffer) {
  write(buffer->getData(), buffer->getSize());
}

// ----------------------------------------------------------------------------
void DynamicBuffer::writeString(const std::string &str) {
  uint16_t strSize = str.size();
  write(&strSize, 2);
  write(str.c_str(), strSize);
}

// ----------------------------------------------------------------------------
uint64_t DynamicBuffer::readUnsigned(uint8_t size) {
  uint64_t number = 0;
  read(&number, size);
  return number;
}

// ----------------------------------------------------------------------------
int64_t DynamicBuffer::readSigned(uint8_t size) {
  int64_t number = 0;
  read(&number, size);
  return number;
}

// ----------------------------------------------------------------------------
std::string DynamicBuffer::readString() {
  uint16_t size = readUnsigned(2);
  char *buffer = (char*)malloc(size + 1);
  buffer[size] = 0;
  if (read(buffer, size) < size) {
    free(buffer);
    return "";
  }

  std::string result(buffer);
  free(buffer);
  return result;
}
