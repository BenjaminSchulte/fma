#include <fma/Project.hpp>
#include <fma/assem/BinaryCodeGenerator.hpp>
#include <fma/assem/BinaryCodeGeneratorScope.hpp>
#include <fma/assem/LabelInstruction.hpp>
#include <fma/linker/LinkerObject.hpp>
#include <fma/linker/LinkerBlock.hpp>
#include <fma/plugin/MemoryPluginAdapter.hpp>
#include <fma/plugin/BinaryGeneratorPluginAdapter.hpp>
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

  for (BinaryGeneratorPluginAdapter *adapter : project->getByteCodeGenerators()) {
    if (!adapter->finish()) {
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
  //target->symbol(name);
  target->setLocation(source->location());

  LabelInstruction label(name);

  BinaryCodeGeneratorScope scope(this, target);
  if (!source->buildByteCode(&scope, &label)) {
    return false;
  }

  return source->buildByteCode(&scope);
}

// ----------------------------------------------------------------------------
