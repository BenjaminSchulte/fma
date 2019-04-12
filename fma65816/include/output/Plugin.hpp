#ifndef __FMA65816_OUTPUT_PLUGIN_H__
#define __FMA65816_OUTPUT_PLUGIN_H__

#include <fma/plugin/Plugin.hpp>
#include <fma/output/DynamicBuffer.hpp>

namespace FMA65816 {
namespace output {

class OutputPlugin : public FMA::plugin::OutputFileWriterPlugin {  
public:
  OutputPlugin(FMA::Project *project);

  const char *getName() const {
    return "SNES ROM generator";
  }

  const char *getDescription() const {
    return "Generates a SNES ROM file";
  }

  bool initialize();
  void release();

  bool generate(FMA::output::OutputAdapter *adapter);

protected:
  void writeByteCode(const FMA::output::DynamicBufferPtr &buffer, FMA::output::OutputAdapter *adapter);
  void setMinimumBufferSize(const FMA::output::DynamicBufferPtr &buffer, FMA::output::OutputAdapter *adapter);
  
  void calculateChecksum(const FMA::output::DynamicBufferPtr &buffer, FMA::output::OutputAdapter *adapter);
  uint16_t getCalculatedChecksum(const FMA::output::DynamicBufferPtr &buffer);
  uint16_t getCalculatedChecksumMirror(const char *data, uint64_t start, uint64_t size, uint32_t mask);
  uint16_t getCalculatedChecksumDefault(const char *data, uint64_t start, uint64_t size);
  
  bool generateOutputFile(const FMA::output::DynamicBufferPtr &buffer, FMA::output::OutputAdapter *adapter);

  uint64_t getRomHeaderAddress(FMA::output::OutputAdapter *adapter);
};

}
}

#endif
