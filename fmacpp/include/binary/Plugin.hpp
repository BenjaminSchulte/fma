#ifndef __FMACPP_BINARY_PLUGIN_H__
#define __FMACPP_BINARY_PLUGIN_H__

#include <fma/plugin/Plugin.hpp>
#include <fma/plugin/BinaryGeneratorPluginAdapter.hpp>

namespace FMACPP {
namespace binary {

class BinaryPlugin : public FMA::plugin::BinaryGeneratorPlugin {  
public:
  BinaryPlugin(FMA::Project *project);

  const char *getName() const {
    return "C++ generator";
  }

  const char *getDescription() const {
    return "Compiles instructions into C++";
  }

  FMA::plugin::BinaryGeneratorPluginAdapter *createAdapter();
  void releaseAdapter(FMA::plugin::BinaryGeneratorPluginAdapter *adapter);

  bool initialize();
  void release();
};

}
}

#endif
