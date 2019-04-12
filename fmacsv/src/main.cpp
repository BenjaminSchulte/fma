#include <fma/plugin/Plugin.hpp>
#include <csv/Plugin.hpp>
#include <string>

using namespace FMA::plugin;

PluginList fmacsv_fmaGetPlugins(FMA::Project *project);

PluginList fmacsv_fmaGetPlugins(FMA::Project *project) {
  PluginList plugins;

  plugins.push_back(PluginPtr(new FMA::csv::CsvPlugin(project)));

  return plugins;
}