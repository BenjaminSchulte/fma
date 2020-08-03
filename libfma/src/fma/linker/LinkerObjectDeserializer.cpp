#include <fma/output/DynamicBuffer.hpp>
#include <fma/linker/LinkerObjectDeserializer.hpp>
#include <fma/linker/LinkerObject.hpp>
#include <fma/linker/LinkerBlock.hpp>
#include <fma/Log.hpp>
#include <fstream>
#include <iostream>

namespace {
  const uint32_t IDENTIFIER = 0xF8400001;
}

using namespace FMA;
using namespace FMA::output;
using namespace FMA::linker;

// ----------------------------------------------------------------------------
bool LinkerObjectDeserializer::deserialize(DynamicBuffer &buffer) const {
  if (buffer.readUnsigned(sizeof(IDENTIFIER)) != IDENTIFIER) {
    mLog->error() << "Invalid file header for fmaobj";
    return false;
  }
  
  if (!deserializeBlocks(buffer)) {
    return false;
  }

  return true;
}

// ----------------------------------------------------------------------------
bool LinkerObjectDeserializer::deserialize(const char *fileName) const {
  std::ifstream file(fileName, std::ios::binary | std::ios::ate);
  if (!file) {
    mLog->error() << "Unable to open file " << fileName;
    return false;
  }

  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);

  char *data = (char*)malloc(size);
  if (!file.read(data, size)) {
    mLog->error() << "Unable to read from file " << fileName;
    free(data);
    return false;
  }

  DynamicBuffer buffer(data, size);
  return deserialize(buffer);
}

// ----------------------------------------------------------------------------
bool LinkerObjectDeserializer::deserializeBlocks(output::DynamicBuffer &buffer) const {
  uint16_t numBlocks = buffer.readUnsigned(2);

  mLog->trace() << "Linker object file contains " << numBlocks << " blocks";
  for (uint16_t i=0; i<numBlocks; i++) {
    if (!deserializeBlock(buffer)) {
      return false;
    }
  }

  return true;
}

// ----------------------------------------------------------------------------
bool LinkerObjectDeserializer::deserializeBlock(output::DynamicBuffer &buffer) const {
  LinkerBlock *block = mObject->createBlock();
  block->setNameHint(buffer.readString());
  mLog->trace() << "Reading block " << block->getNameHint();

  uint32_t dataSize = buffer.readUnsigned(4);
  char *data = (char*)malloc(dataSize);
  buffer.read(data, dataSize);
  block->write(data, dataSize);
  free(data);

  uint8_t hasLocation = buffer.readUnsigned(1);
  if (hasLocation) {
    mLog->error() << "TODO: hasLocation";
    return false;
  }

  uint32_t numSymbols = buffer.readUnsigned(4);
  for (uint32_t i=0; i<numSymbols; i++) {
    LinkerBlockSymbol sym;
    sym.name = buffer.readString();
    sym.offset = buffer.readUnsigned(sizeof(sym.offset));
    block->addSymbol(sym);
  }

  uint32_t numRefs = buffer.readUnsigned(4);
  for (uint32_t i=0; i<numRefs; i++) {
    LinkerBlockReference ref;
    ref.reference = FMA::symbol::Reference::deserialize(mLog, buffer);
    if (!ref.reference) {
      return false;
    }
    
    ref.offset = buffer.readUnsigned(sizeof(ref.offset));
    ref.size = buffer.readUnsigned(sizeof(ref.size));
    block->addReference(ref);
  }

  return true;
}
