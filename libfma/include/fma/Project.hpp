#ifndef __FMA_PROJECT_H__
#define __FMA_PROJECT_H__

#include "Log.hpp"
#include <memory>
#include <vector>

namespace FMA {
class ProjectQueue;
namespace serialize {
class SerializerRegistry;
}
namespace types {
  class RootModule;
}
namespace plugin {
  class MemoryPluginAdapter;
  class BinaryGeneratorPluginAdapter;
}

typedef std::vector<plugin::BinaryGeneratorPluginAdapter*> BinaryGeneratorAdapterList;

class Project {
public:
  Project();
  virtual ~Project();

  inline void setTargetName(const std::string &name) { targetName = name; }
  const std::string &getTargetName() const { return targetName; }

  inline class FileMap *getFiles() const { return files; }
  inline Log &log() const { return *_log; }

  inline bool hasErrors() const { return _log->hasErrors(); }

  inline void setMemoryAdapter(plugin::MemoryPluginAdapter *adapter) { memoryAdapter = adapter; }
  inline plugin::MemoryPluginAdapter *getMemoryAdapter() const { return memoryAdapter; }

  void addByteCodeGenerator(plugin::BinaryGeneratorPluginAdapter*);
  inline const BinaryGeneratorAdapterList &getByteCodeGenerators() const { return binaryAdapters; }
  void removeAllByteCodeGenerators();

  inline const std::shared_ptr<types::RootModule> &root() const { return _root; }
  inline serialize::SerializerRegistry *serializer() const { return _serializer; }

protected:
  class FileMap *files;
  plugin::MemoryPluginAdapter *memoryAdapter = nullptr;
  BinaryGeneratorAdapterList binaryAdapters;
  Log *_log;
  ConsoleLog *_defaultLogAdapter;
  serialize::SerializerRegistry *_serializer;
  std::shared_ptr<types::RootModule> _root;
  std::string targetName;
};

}

#endif