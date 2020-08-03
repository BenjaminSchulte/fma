#include <fma/output/DynamicBuffer.hpp>
#include <fma/linker/LinkerObjectSerializer.hpp>
#include <fma/linker/LinkerObject.hpp>
#include <fma/linker/LinkerBlock.hpp>
#include <iostream>

namespace {
  const uint32_t IDENTIFIER = 0xF8400001;
}

using namespace FMA;
using namespace FMA::linker;

// ----------------------------------------------------------------------------
bool LinkerObjectSerializer::serialize() {
  mBuffer.write(&IDENTIFIER, sizeof(IDENTIFIER));

  if (!serializeBlocks()) {
    return false;
  }

  return true;
}

// ----------------------------------------------------------------------------
bool LinkerObjectSerializer::serializeBlocks() {
  const auto &blocks = mObject->getBlocks();
  uint16_t numBlocks = blocks.size();
  mBuffer.write(&numBlocks, 2);

  for (const LinkerBlock *block : blocks) {
    if (!serializeBlock(block)) {
      return false;
    }
  }

  return true;
}

// ----------------------------------------------------------------------------
bool LinkerObjectSerializer::serializeBlock(const LinkerBlock *block) {
  mBuffer.writeString(block->getNameHint());

  uint32_t size = block->getSize();
  mBuffer.write(&size, 4);
  mBuffer.write(block->getData(), size);

  uint8_t hasLocation = block->hasLocation() ? 1 : 0;
  mBuffer.write(&hasLocation, 1);
  if (hasLocation) {
    if (!block->location()->serialize(mBuffer)) {
      return false;
    }
  }

  uint32_t numSymbols = block->getSymbols().size();
  mBuffer.write(&numSymbols, 4);
  for (const auto &symbol : block->getSymbols()) {
    mBuffer.writeString(symbol.name);
    mBuffer.write(&symbol.offset, sizeof(symbol.offset));
  }

  uint32_t numReferences = block->getReferences().size();
  mBuffer.write(&numReferences, 4);
  for (const auto &reference : block->getReferences()) {
    if (!reference.reference->serialize(mBuffer)) {
      return false;
    }
    mBuffer.write(&reference.offset, sizeof(reference.offset));
    mBuffer.write(&reference.size, sizeof(reference.size));
  }

  return true;
}

// ----------------------------------------------------------------------------
