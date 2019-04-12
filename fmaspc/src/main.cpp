#include <fma/plugin/Plugin.hpp>
#include <lang/Plugin.hpp>
#include <binary/Plugin.hpp>
#include <output/Plugin.hpp>
#include <sym/Plugin.hpp>
#include <string>

using namespace FMA::plugin;

PluginList fmaspc_fmaGetPlugins(FMA::Project *project);

PluginList fmaspc_fmaGetPlugins(FMA::Project *project) {
  PluginList plugins;
  
  plugins.push_back(PluginPtr(new FMASPC::lang::SpcLanguagePlugin(project)));
  plugins.push_back(PluginPtr(new FMASPC::binary::SpcBinaryPlugin(project)));
  plugins.push_back(PluginPtr(new FMASPC::output::SpcOutputPlugin(project)));
  plugins.push_back(PluginPtr(new FMASPC::sym::SpcOutputPlugin(project)));

  return plugins;
}