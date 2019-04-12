#ifndef __FMASPC_SYM_PLUGIN_H__
#define __FMASPC_SYM_PLUGIN_H__

#include <fma/plugin/Plugin.hpp>
#include <fma/output/DynamicBuffer.hpp>

namespace FMASPC {
namespace sym {

class SpcOutputPlugin : public FMA::plugin::OutputFileWriterPlugin {  
public:
  SpcOutputPlugin(FMA::Project *project);

  const char *getName() const {
    return "SPC symbol file generator";
  }

  const char *getDescription() const {
    return "Generates a SPC symbol file";
  }

  bool initialize();
  void release();

  bool generate(FMA::output::OutputAdapter *adapter);
};

}
}

#endif
