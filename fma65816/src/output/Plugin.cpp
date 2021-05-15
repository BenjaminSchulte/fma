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
using namespace FMA65816::output;

// ----------------------------------------------------------------------------
OutputPlugin::OutputPlugin(Project *project)
  : OutputFileWriterPlugin(project)
{
}

// ----------------------------------------------------------------------------
bool OutputPlugin::initialize() {
  return true;
}

// ----------------------------------------------------------------------------
void OutputPlugin::writeByteCode(const DynamicBufferPtr &buffer, OutputAdapter *adapter) {
  for (const auto &block : adapter->getObject()->getBlocks()) {
    uint64_t address = block->getPlacement()->asTranslatedAddress();

    buffer->seek(address);
    buffer->write(block->getData(), block->getSize());
  }
}

// ----------------------------------------------------------------------------
bool OutputPlugin::generateOutputFile(const DynamicBufferPtr &buffer, OutputAdapter *adapter) {
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
void OutputPlugin::setMinimumBufferSize(const DynamicBufferPtr &buffer, OutputAdapter *adapter) {
  uint64_t size = buffer->getSize();
  uint64_t currentTargetSize = 2 * 1024 * 1024 / 8;
  uint8_t currentTargetByte = 0x08;

  while (size > currentTargetSize) {
    currentTargetSize *= 2;
    currentTargetByte++;
  }

  buffer->seek(getRomHeaderAddress(adapter) + 0x27);
  buffer->write(&currentTargetByte, 1);

  buffer->forceSize(currentTargetSize);

  project->log().debug() << "Using ROM size configuration " << (int)currentTargetByte << " size " << currentTargetSize;
}

// ----------------------------------------------------------------------------
uint16_t OutputPlugin::getCalculatedChecksumMirror(const char *data, uint64_t start, uint64_t size, uint32_t mask) {
  while (!(size & mask)) {
    if (mask == 0) {
      std::cerr << "Error while calculating checksum" << std::endl;
      return 0;
    }

    mask >>= 1;
  }

  uint16_t part1 = getCalculatedChecksumDefault(data, start, mask);
  uint16_t part2 = 0;

  uint64_t nextLength = size - mask;
  if (nextLength) {
    part2 = getCalculatedChecksumDefault(data, start + mask, nextLength);

    while (nextLength < mask) {
      nextLength += nextLength;
      part2 += part2;
    }
  }

  return part1 + part2;
}

// ----------------------------------------------------------------------------
uint16_t OutputPlugin::getCalculatedChecksumDefault(const char *data, uint64_t start, uint64_t size) {
  uint16_t result = 0;
  while (size--) {
    result += data[start++];
  }
  return result;
}

// ----------------------------------------------------------------------------
uint16_t OutputPlugin::getCalculatedChecksum(const DynamicBufferPtr &buffer) {
  return getCalculatedChecksumMirror(reinterpret_cast<const char *>(buffer->getData()), 0, buffer->getSize(), 0x800000);
}

// ----------------------------------------------------------------------------
void OutputPlugin::calculateChecksum(const DynamicBufferPtr &buffer, OutputAdapter *adapter) {
  uint16_t checksum = getCalculatedChecksum(buffer);
  uint16_t notChecksum = checksum ^ 0xFFFF;

  buffer->seek(getRomHeaderAddress(adapter) + 0x2C);
  buffer->write(&notChecksum, 2);
  buffer->write(&checksum, 2);
}

// ----------------------------------------------------------------------------
bool OutputPlugin::generate(OutputAdapter *adapter) {
  project->log().info() << "Building SNES ROM output binary.";

  DynamicBufferPtr buffer(new FMA::output::DynamicBuffer());
  writeByteCode(buffer, adapter);
  setMinimumBufferSize(buffer, adapter);
  calculateChecksum(buffer, adapter);
  
  return generateOutputFile(buffer, adapter);
}

// ----------------------------------------------------------------------------
uint64_t OutputPlugin::getRomHeaderAddress(OutputAdapter *adapter) {
  return adapter->getProject()->getMemoryAdapter()->translateAddress(0xFFB0);
}

// ----------------------------------------------------------------------------
void OutputPlugin::release() {
}
