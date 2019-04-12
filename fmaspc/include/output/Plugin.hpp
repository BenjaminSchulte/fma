#ifndef __FMASPC_OUTPUT_PLUGIN_H__
#define __FMASPC_OUTPUT_PLUGIN_H__

#include <fma/plugin/Plugin.hpp>
#include <fma/output/DynamicBuffer.hpp>

namespace FMASPC {
namespace output {

class SpcOutputPlugin : public FMA::plugin::OutputFileWriterPlugin {  
public:
  SpcOutputPlugin(FMA::Project *project);

  const char *getName() const {
    return "SPC ROM generator";
  }

  const char *getDescription() const {
    return "Generates a SPC ROM file";
  }

  bool initialize();
  void release();

  bool generate(FMA::output::OutputAdapter *adapter);

protected:
  void writeByteCode(const FMA::output::DynamicBufferPtr &buffer, FMA::output::OutputAdapter *adapter);
  bool generateOutputFile(const FMA::output::DynamicBufferPtr &buffer, FMA::output::OutputAdapter *adapter);
};

}
}

#endif
