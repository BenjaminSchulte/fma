#include <fma/plugin/Plugin.hpp>
#include <json/Plugin.hpp>
#include <string>

using namespace FMA::plugin;

PluginList fmajson_fmaGetPlugins(FMA::Project *project);

PluginList fmajson_fmaGetPlugins(FMA::Project *project) {
  PluginList plugins;

  plugins.push_back(PluginPtr(new FMA::json::JsonPlugin(project)));

  return plugins;
}