#include <fma/linker/Linker.hpp>
#include <fma/linker/LinkerObject.hpp>
#include <fma/linker/LinkerBlock.hpp>
#include <fma/plugin/MemoryPluginAdapter.hpp>
#include <fma/Project.hpp>
#include <iostream>

using namespace FMA;
using namespace FMA::plugin;
using namespace FMA::linker;

// ----------------------------------------------------------------------------
Linker::Linker(Project *project, LinkerObject *object)
  : project(project)
  , object(object)
{
}

// ----------------------------------------------------------------------------
Linker::~Linker() {
}

// ----------------------------------------------------------------------------
bool Linker::link() {
  project->log().info() << "Linking RAM.";
  if (!project->getMemoryAdapter()->placeDynamicBlocks()) {
    return false;
  }
  
  project->log().info() << "Linking ROM.";
  for (auto &block : object->getBlocks()) {
    auto *placement = project->getMemoryAdapter()->requireStaticBlockPlacement(block);
    block->setPlacement(placement);
  }

  if (!project->getMemoryAdapter()->placeStaticBlocks()) {
    return false;
  }

  project->log().info() << "Resolving symbols.";
  project->log().debug() << "Collecting symbols in all blocks.";
  bool registerLinkerBlocksSuceeded = true;
  for (auto &block : object->getBlocks()) {
    if (!project->getMemoryAdapter()->registerLinkerBlockSymbols(block)) {
      registerLinkerBlocksSuceeded = false;
    }
  }
  if (!registerLinkerBlocksSuceeded) {
    return false;
  }

  project->log().debug() << "Replacing symbols in all blocks.";
  for (auto &block : object->getBlocks()) {
    if (!block->calculateSymbols(project, project->getMemoryAdapter()->getSymbolMap())) {
      return false;
    }
  }

  project->log().info() << "Successfully linked project.";
  return true;
}

// ----------------------------------------------------------------------------
