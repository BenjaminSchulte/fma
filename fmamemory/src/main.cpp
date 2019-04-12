#include <fma/plugin/Plugin.hpp>
#include <memory/Plugin.hpp>
#include <string>

using namespace FMA::plugin;

PluginList fmamemory_fmaGetPlugins(FMA::Project *project);

PluginList fmamemory_fmaGetPlugins(FMA::Project *project) {
  PluginList plugins;
  
  plugins.push_back(PluginPtr(new FMA::memory::MemoryPlugin(project)));
  
  return plugins;
}