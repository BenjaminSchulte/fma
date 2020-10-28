#include <fma/plugin/Plugin.hpp>
#include <lang/Plugin.hpp>
#include <binary/Plugin.hpp>
#include <output/Plugin.hpp>
#include <sym/Plugin.hpp>
#include <bp/Plugin.hpp>
#include <string>

using namespace FMA::plugin;

PluginList fma65816_fmaGetPlugins(FMA::Project *project);

PluginList fma65816_fmaGetPlugins(FMA::Project *project) {
  PluginList plugins;
  
  plugins.push_back(PluginPtr(new FMA65816::lang::LanguagePlugin(project)));
  plugins.push_back(PluginPtr(new FMA65816::binary::BinaryPlugin(project)));
  plugins.push_back(PluginPtr(new FMA65816::output::OutputPlugin(project)));
  plugins.push_back(PluginPtr(new FMA65816::sym::OutputPlugin(project)));
  plugins.push_back(PluginPtr(new FMA65816::bp::OutputPlugin(project)));
  
  return plugins;
}