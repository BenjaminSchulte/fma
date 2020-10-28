#ifndef __FMA65816_BP_PLUGIN_H__
#define __FMA65816_BP_PLUGIN_H__

#include <fma/plugin/Plugin.hpp>
#include <fma/output/DynamicBuffer.hpp>

namespace FMA65816 {
namespace bp {

class OutputPlugin : public FMA::plugin::OutputFileWriterPlugin {  
public:
  OutputPlugin(FMA::Project *project);

  const char *getName() const {
    return "SNES breakpoint file generator";
  }

  const char *getDescription() const {
    return "Generates a BSNES breakpoint file";
  }

  bool initialize();
  void release();

  bool generate(FMA::output::OutputAdapter *adapter);
};

}
}

#endif
