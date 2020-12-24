#ifndef __FMA65816_SCHEMA_PLUGIN_H__
#define __FMA65816_SCHEMA_PLUGIN_H__

#include <fma/plugin/Plugin.hpp>
#include <fma/output/DynamicBuffer.hpp>

namespace FMA {
namespace memory {

class OutputSchemaPlugin : public FMA::plugin::OutputFileWriterPlugin {  
public:
  OutputSchemaPlugin(FMA::Project *project);

  const char *getName() const {
    return "Schema file generator";
  }

  const char *getDescription() const {
    return "Generates a BSNES schema file";
  }

  bool initialize();
  void release();

  bool generate(FMA::output::OutputAdapter *adapter);
};

}
}

#endif
