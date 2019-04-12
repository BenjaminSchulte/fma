#include <fma/plugin/BinaryGeneratorPluginAdapter.hpp>

#include <binary/PluginAdapter.hpp>
#include <binary/Plugin.hpp>

using namespace FMA;
using namespace FMA::plugin;
using namespace FMASPC::binary;

// ----------------------------------------------------------------------------
SpcBinaryPlugin::SpcBinaryPlugin(Project *project)
  : BinaryGeneratorPlugin(project)
{
}

// ----------------------------------------------------------------------------
bool SpcBinaryPlugin::initialize() {
  return true;
}

// ----------------------------------------------------------------------------
void SpcBinaryPlugin::release() {
}

// ----------------------------------------------------------------------------
BinaryGeneratorPluginAdapter *SpcBinaryPlugin::createAdapter() {
  return new SpcPluginAdapter(project);
}

// ----------------------------------------------------------------------------
void SpcBinaryPlugin::releaseAdapter(BinaryGeneratorPluginAdapter *adapter) {
  delete adapter;
}

// ----------------------------------------------------------------------------