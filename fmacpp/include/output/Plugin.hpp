#pragma once

#include <fma/plugin/Plugin.hpp>
#include <fma/output/DynamicBuffer.hpp>

namespace FMACPP {
namespace output {

class OutputPlugin : public FMA::plugin::OutputFileWriterPlugin {  
public:
  OutputPlugin(FMA::Project *project);

  const char *getName() const {
    return "SNES C++ code generator";
  }

  const char *getDescription() const {
    return "Generates a SNES C++ code file";
  }

  bool initialize();
  void release();

  bool generate(FMA::output::OutputAdapter *adapter);
};

}
}

