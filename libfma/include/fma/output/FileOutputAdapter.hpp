#ifndef __FMA_PLUGIN_FILEOUTPUTPLUGINADAPTER_H__
#define __FMA_PLUGIN_FILEOUTPUTPLUGINADAPTER_H__

#include <string>
#include <stdio.h>
#include "OutputAdapter.hpp"

namespace FMA {
namespace output {

class FileBuffer : public Buffer {
public:
  FileBuffer(FILE *);
  virtual ~FileBuffer();

  virtual bool write(const void *data, size_t size);

protected:
  FILE *fh;
};

class FileOutputAdapter : public OutputAdapter {
public:
  FileOutputAdapter(Project *project, linker::LinkerObject *object, const std::string &path, const std::string &name);

  BufferPtr open(bool binary=true);
  BufferPtr open(const std::string &name, bool binary=true);
  BufferPtr openWithExtension(const std::string &name, bool binary=true);

protected:
  std::string path;
  std::string name;
};

}
}

#endif
