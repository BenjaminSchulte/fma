#ifndef __FMASPC_BINARY_PLUGIN_H__
#define __FMASPC_BINARY_PLUGIN_H__

#include <fma/plugin/Plugin.hpp>
#include <fma/plugin/BinaryGeneratorPluginAdapter.hpp>

namespace FMASPC {
namespace binary {

class SpcBinaryPlugin : public FMA::plugin::BinaryGeneratorPlugin {  
public:
  SpcBinaryPlugin(FMA::Project *project);

  const char *getName() const {
    return "SPC binary generator";
  }

  const char *getDescription() const {
    return "Compiles instructions into bytecode";
  }

  FMA::plugin::BinaryGeneratorPluginAdapter *createAdapter();
  void releaseAdapter(FMA::plugin::BinaryGeneratorPluginAdapter *adapter);

  bool initialize();
  void release();

protected:
  FMA::Project *project;
};

}
}

#endif
