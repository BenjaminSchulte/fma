#include <fma/plugin/Plugin.hpp>
#include <image/Plugin.hpp>
#include <string>

using namespace FMA::plugin;

PluginList fmaimage_fmaGetPlugins(FMA::Project *project);

PluginList fmaimage_fmaGetPlugins(FMA::Project *project) {
  PluginList plugins;
  
  plugins.push_back(PluginPtr(new FMA::image::ImagePlugin(project)));

  return plugins;
}