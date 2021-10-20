#include <superfx/Plugin.hpp>
#include <fma/plugin/Plugin.hpp>
#include <string>

using namespace FMA::plugin;

PluginList fmasuperfx_fmaGetPlugins(FMA::Project *project);

PluginList fmasuperfx_fmaGetPlugins(FMA::Project *project) {
  PluginList plugins;
  
  plugins.push_back(PluginPtr(new FMA::superfx::SuperFxPlugin(project)));
  
  return plugins;
}