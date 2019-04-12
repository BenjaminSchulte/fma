#ifndef __FMA_APPLICATION_H__
#define __FMA_APPLICATION_H__

#include <fma/Project.hpp>
#include <fma/linker/LinkerObject.hpp>
#include <fma/plugin/Plugin.hpp>
#include <fma/plugin/MemoryPluginAdapter.hpp>

#include "Options.hpp"

typedef std::shared_ptr<FMA::plugin::Plugin> PluginPtr;
typedef std::shared_ptr<FMA::plugin::MemoryManagerPlugin> MemoryPluginPtr;
typedef std::shared_ptr<FMA::plugin::OutputFileWriterPlugin> OutputFileWriterPluginPtr;
typedef std::vector<PluginPtr> PluginList;

class Application {
public:
  Application();
  ~Application();

  int run(int argc, char **argv);
  bool loadPlugin(const PluginPtr &);

  bool parseAndInterpretFile(const std::string &file);

protected:
  bool loadPlugins();
  bool loadOptions();
  bool loadLanguagePlugins();
  bool loadMemoryPlugins();
  bool loadPlugins(const PluginList &);
  void unloadPlugins();
  
  bool parseAndInterpretFiles();
  bool generateRom(FMA::linker::LinkerObject *object);
  FMA::linker::LinkerObject *buildBinaryCode();
  bool link(FMA::linker::LinkerObject *object);
  int runAfterInitialize();

  Options options;
  PluginList plugins;
  std::vector<OutputFileWriterPluginPtr> outputFileWriters;
  FMA::Project project;
};

#endif
