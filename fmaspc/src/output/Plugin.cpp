#include <output/Plugin.hpp>
#include <fma/Project.hpp>
#include <fma/output/DynamicBuffer.hpp>
#include <fma/output/OutputAdapter.hpp>
#include <fma/linker/LinkerObject.hpp>
#include <fma/linker/LinkerBlock.hpp>
#include <iostream>

using namespace FMA;
using namespace FMA::output;
using namespace FMA::linker;
using namespace FMASPC::output;

// ----------------------------------------------------------------------------
SpcOutputPlugin::SpcOutputPlugin(Project *project)
  : OutputFileWriterPlugin(project)
{
}

// ----------------------------------------------------------------------------
bool SpcOutputPlugin::initialize() {
  return true;
}

// ----------------------------------------------------------------------------
void SpcOutputPlugin::writeByteCode(const DynamicBufferPtr &buffer, OutputAdapter *adapter) {
  for (const auto &block : adapter->getObject()->getBlocks()) {
    uint64_t address = block->getPlacement()->asTranslatedAddress();

    buffer->seek(address);
    buffer->write(block->getData(), block->getSize());
  }
}

// ----------------------------------------------------------------------------
bool SpcOutputPlugin::generateOutputFile(const DynamicBufferPtr &buffer, OutputAdapter *adapter) {
  BufferPtr file = adapter->open();
  if (!file) {
    project->log().error() << "Unable to open output file.";
    return false;
  }

  if (!file->write(buffer)) {
    project->log().error() << "Unable to write bytes to buffer.";
    return false;
  }

  return true;
}

// ----------------------------------------------------------------------------
bool SpcOutputPlugin::generate(OutputAdapter *adapter) {
  project->log().info() << "Building SPC ROM output binary.";

  DynamicBufferPtr buffer(new DynamicBuffer());
  writeByteCode(buffer, adapter);
  
  return generateOutputFile(buffer, adapter);
}

// ----------------------------------------------------------------------------
void SpcOutputPlugin::release() {
}
