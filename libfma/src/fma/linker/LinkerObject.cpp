#include <fma/linker/LinkerObject.hpp>
#include <fma/linker/LinkerBlock.hpp>

using namespace FMA::linker;

// ----------------------------------------------------------------------------
LinkerObject::LinkerObject() {

}

// ----------------------------------------------------------------------------
LinkerObject::~LinkerObject() {
  removeAllBlocks();
}

// ----------------------------------------------------------------------------
LinkerBlock *LinkerObject::createBlock() {
  LinkerBlock *block = new LinkerBlock();
  blocks.push_back(block);
  return block;
}

// ----------------------------------------------------------------------------
void LinkerObject::removeAllBlocks() {
  for (auto &block : blocks) {
    delete block;
  }

  blocks.clear();
}

// ----------------------------------------------------------------------------
