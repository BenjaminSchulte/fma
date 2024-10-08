#include <fma/plugin/BinaryGeneratorPluginAdapter.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/Project.hpp>

#include <binary/PluginAdapter.hpp>
#include <binary/Plugin.hpp>

using namespace FMA;
using namespace FMA::plugin;
using namespace FMACPP::binary;

// ----------------------------------------------------------------------------
BinaryPlugin::BinaryPlugin(Project *project)
  : BinaryGeneratorPlugin(project)
{
}

// ----------------------------------------------------------------------------
bool BinaryPlugin::initialize() {
  return true;
}

// ----------------------------------------------------------------------------
void BinaryPlugin::release() {
}

// ----------------------------------------------------------------------------
BinaryGeneratorPluginAdapter *BinaryPlugin::createAdapter() {
  return new PluginAdapter(project);
}

// ----------------------------------------------------------------------------
void BinaryPlugin::releaseAdapter(BinaryGeneratorPluginAdapter *adapter) {
  delete adapter;
}

// ----------------------------------------------------------------------------