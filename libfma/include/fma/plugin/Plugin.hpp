#ifndef __FMA_PLUGIN_H__
#define __FMA_PLUGIN_H__

#include <vector>
#include <memory>

namespace FMA {
class Project;
namespace output {
class OutputAdapter;
}
namespace plugin {

class MemoryPluginAdapter;
class BinaryGeneratorPluginAdapter;

enum PluginType {
  // Extends the language with new features
  TYPE_LANGUAGE = 1,

  // Adds allocation support to the language
  TYPE_MEMORY_MANAGER = 2,

  // Adds support to generate binary blobs from instructions
  TYPE_BINARY_BUILDER = 3,

  // Generates the output file
  TYPE_OUTPUT_FILE_BUILDER = 4,
};

class Plugin {
public:
  Plugin(Project *project) : project(project) {}
  virtual ~Plugin() {}

  virtual PluginType getPluginType() const = 0;

  virtual const char *getName() const = 0;
  virtual const char *getDescription() const = 0;

  virtual bool initialize() = 0;
  virtual void release() = 0;

protected:
  Project *project;
};


class LanguagePlugin : public Plugin {
public:
  LanguagePlugin(Project *project) : Plugin(project) {}

  virtual PluginType getPluginType() const {
    return TYPE_LANGUAGE;
  };
};

class BinaryGeneratorPlugin : public Plugin {
public:
  BinaryGeneratorPlugin(Project *project) : Plugin(project) {}

  virtual BinaryGeneratorPluginAdapter *createAdapter() = 0;
  virtual void releaseAdapter(BinaryGeneratorPluginAdapter *adapter) = 0;

  virtual PluginType getPluginType() const {
    return TYPE_BINARY_BUILDER;
  };
};

class MemoryManagerPlugin : public Plugin {
public:
  MemoryManagerPlugin(Project *project) : Plugin(project) {}

  virtual MemoryPluginAdapter *createAdapter() = 0;
  virtual void releaseAdapter(MemoryPluginAdapter *adapter) = 0;

  virtual PluginType getPluginType() const {
    return TYPE_MEMORY_MANAGER;
  };
};

class OutputFileWriterPlugin : public Plugin {
public:
  OutputFileWriterPlugin(Project *project) : Plugin(project) {}

  virtual bool generate(output::OutputAdapter *adapter) = 0;

  virtual PluginType getPluginType() const {
    return TYPE_OUTPUT_FILE_BUILDER;
  };
};

typedef std::shared_ptr<Plugin> PluginPtr;
typedef std::vector<PluginPtr> PluginList;

}
}

#endif
 