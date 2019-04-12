#ifndef __FMA_OUTPUT_OUTPUTADAPTER_H__
#define __FMA_OUTPUT_OUTPUTADAPTER_H__

#include "DynamicBuffer.hpp"
#include "../plugin/MemoryPluginAdapter.hpp"

namespace FMA {
class Project;  
namespace linker {
class LinkerObject;
}
namespace output {

class Buffer {
public:
  Buffer() {};
  virtual ~Buffer() {};

  virtual bool write(const void *data, size_t size) = 0;
  virtual bool write(const std::string &str) {
    return write(str.c_str(), str.size());
  }
  virtual bool write(const DynamicBufferPtr &buffer) {
    return write(buffer->getData(), buffer->getSize());
  }
};
typedef std::shared_ptr<Buffer> BufferPtr;

class OutputAdapter {
public:
  OutputAdapter(Project *project, linker::LinkerObject *object);
  virtual ~OutputAdapter();

  inline Project *getProject() const { return project; }
  inline linker::LinkerObject *getObject() const { return object; }

  const plugin::MemorySymbolMap *getSymbolMap();
  
  virtual BufferPtr open(bool binary=true) = 0;
  virtual BufferPtr open(const std::string &name, bool binary=true) = 0;
  virtual BufferPtr openWithExtension(const std::string &name, bool binary=true) = 0;

protected:
  Project *project;
  linker::LinkerObject *object;
};

}
}

#endif
