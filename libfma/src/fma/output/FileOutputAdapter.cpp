#include <boost/filesystem.hpp>
#include <fma/linker/LinkerObject.hpp>
#include <fma/plugin/Plugin.hpp>
#include <fma/output/FileOutputAdapter.hpp>

using namespace FMA;
using namespace FMA::output;
using namespace FMA::linker;


// ----------------------------------------------------------------------------
FileBuffer::FileBuffer(FILE *fh)
  : fh(fh)
{
}

// ----------------------------------------------------------------------------
FileBuffer::~FileBuffer() {
  fclose(fh);
}

// ----------------------------------------------------------------------------
bool FileBuffer::write(const void *data, size_t size) {
  size_t written = fwrite(data, 1, size, fh);
  if (written != size) {
    return false;
  }

  return true;
}


// ----------------------------------------------------------------------------
FileOutputAdapter::FileOutputAdapter(Project *project, LinkerObject *object, const std::string &path, const std::string &name)
  : OutputAdapter(project, object)
  , path(path)
  , name(name)
{
}

// ----------------------------------------------------------------------------
BufferPtr FileOutputAdapter::open(bool binary) {
  return open(name, binary);
}

// ----------------------------------------------------------------------------
BufferPtr FileOutputAdapter::openWithExtension(const std::string &postfix, bool binary) {
  boost::filesystem::path p(name);  
  return open(p.stem().string() + postfix, binary);
}

// ----------------------------------------------------------------------------
BufferPtr FileOutputAdapter::open(const std::string &name, bool binary) {
  std::string fullPath = (boost::filesystem::path(path) / name).string();

  FILE *fh = fopen(fullPath.c_str(), binary ? "wb" : "w");
  if (!fh) {
    return BufferPtr();
  }

  return BufferPtr(new FileBuffer(fh));
}

// ----------------------------------------------------------------------------
