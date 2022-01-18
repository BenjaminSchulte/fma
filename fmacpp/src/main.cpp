#include <fma/plugin/Plugin.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/Project.hpp>
#include <binary/Plugin.hpp>
#include <output/Plugin.hpp>

using namespace FMA::plugin;

PluginList fmacpp_fmaGetPlugins(FMA::Project *project);

PluginList fmacpp_fmaGetPlugins(FMA::Project *project) {
  PluginList plugins;

  plugins.push_back(PluginPtr(new FMACPP::binary::BinaryPlugin(project)));
  plugins.push_back(PluginPtr(new FMACPP::output::OutputPlugin(project)));
  return plugins;
}