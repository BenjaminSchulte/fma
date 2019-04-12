#include <fma/plugin/Plugin.hpp>
#include <fs/Plugin.hpp>
#include <string>

using namespace FMA::plugin;

PluginList fmafs_fmaGetPlugins(FMA::Project *project);

PluginList fmafs_fmaGetPlugins(FMA::Project *project) {
  PluginList plugins;

  plugins.push_back(PluginPtr(new FMA::fs::FsPlugin(project)));

  return plugins;
}