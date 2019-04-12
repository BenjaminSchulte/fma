#ifndef __FMA65816_BINARY_PLUGIN_H__
#define __FMA65816_BINARY_PLUGIN_H__

#include <fma/plugin/Plugin.hpp>
#include <fma/plugin/BinaryGeneratorPluginAdapter.hpp>

namespace FMA65816 {
namespace binary {

class BinaryPlugin : public FMA::plugin::BinaryGeneratorPlugin {  
public:
  BinaryPlugin(FMA::Project *project);

  const char *getName() const {
    return "65816 binary generator";
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
