#include <fma/Project.hpp>
#include <fma/assem/BinaryCodeGenerator.hpp>
#include <fma/assem/BinaryCodeGeneratorScope.hpp>
#include <fma/linker/LinkerObject.hpp>
#include <fma/linker/LinkerBlock.hpp>
#include <fma/plugin/MemoryPluginAdapter.hpp>
#include <fma/symbol/SymbolReference.hpp>
#include <iostream>

using namespace FMA;
using namespace FMA::assem;
using namespace FMA::symbol;
using namespace FMA::plugin;
using namespace FMA::linker;

// ----------------------------------------------------------------------------
BinaryCodeGenerator::BinaryCodeGenerator(Project *project, LinkerObject *object)
  : project(project)
  , object(object)
{
}

// ----------------------------------------------------------------------------
BinaryCodeGenerator::~BinaryCodeGenerator() {
}

// ----------------------------------------------------------------------------
void BinaryCodeGenerator::add(MemoryBlock *block) {
  blocks.push_back(block);
}

// ----------------------------------------------------------------------------
bool BinaryCodeGenerator::generate() {
  while (blocks.size()) {
    auto block = blocks.back();
    blocks.pop_back();

    if (!generate(block)) {
      return false;
    }
  }

  return true;
}

// ----------------------------------------------------------------------------
bool BinaryCodeGenerator::generate(MemoryBlock *source) {
  const std::string &name = source->createReference()->getName();
  project->log().trace() << "Generate bytecode for " << name;

  LinkerBlock *target = object->createBlock();
  target->symbol(name);
  target->setLocation(source->location());

  BinaryCodeGeneratorScope scope(this, target);
  return source->buildByteCode(&scope);
}

// ----------------------------------------------------------------------------
