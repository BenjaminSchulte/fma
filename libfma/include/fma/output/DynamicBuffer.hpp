#ifndef __FMA_OUTPUT_DYNAMICBUFFER_H__
#define __FMA_OUTPUT_DYNAMICBUFFER_H__

#include <cstdint>
#include <memory>

namespace FMA {
namespace output {

typedef std::shared_ptr<class DynamicBuffer> DynamicBufferPtr;

class DynamicBuffer {
public:
  DynamicBuffer();
  DynamicBuffer(void *data, uint32_t size);
  virtual ~DynamicBuffer();

  void rewind();
  void seek(uint32_t offset);
  inline uint32_t tell() const { return offset; }
  void write(const void *data, uint32_t size);
  void writeNumber(int64_t number, uint32_t size);
  void writeString(const std::string &);
  uint32_t read(void *data, uint32_t size);
  uint32_t read(uint32_t offset, void *data, uint32_t size);
  void write(const DynamicBufferPtr &buffer);
  void forceSize(uint32_t size);

  uint64_t readUnsigned(uint8_t size);
  int64_t readSigned(uint8_t size);
  std::string readString();

  inline void *getData() const { return data; }
  inline uint32_t getSize() const { return size; }
  inline uint32_t getMemorySize() const { return memorySize; }

protected:
  void allocate(uint32_t size);

  void *data;
  uint32_t offset;
  uint32_t size;
  uint32_t memorySize;
};

struct DynamicBufferAddress {
public:
  DynamicBufferAddress(const DynamicBufferPtr &buffer, uint32_t offset) : buffer(buffer), offset(offset), firstOffset(offset) {}

  inline void seek(uint32_t _offset) { offset = firstOffset + _offset; }
  inline uint32_t tell() const { return offset; }

  inline uint32_t read(void *data, uint32_t size) {
    size = buffer->read(offset, data, size);
    offset += size;
    return size;
  }

protected:
  DynamicBufferPtr buffer;
  uint32_t offset;
  uint32_t firstOffset;
};

}
}

#endif
