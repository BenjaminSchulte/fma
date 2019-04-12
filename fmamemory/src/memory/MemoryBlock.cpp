#include <memory/MemoryBlock.hpp>
#include <memory/MemoryAdapter.hpp>
#include <memory/MemorySymbolMap.hpp>
#include <fma/assem/Instruction.hpp>
#include <fma/assem/LabelInstruction.hpp>
#include <fma/assem/BinaryCodeGeneratorScope.hpp>
#include <fma/plugin/BinaryGeneratorPluginAdapter.hpp>
#include <fma/symbol/Reference.hpp>
#include <fma/symbol/SymbolReference.hpp>
#include <fma/instruct/BinaryData.hpp>
#include <fma/instruct/DynamicBinaryData.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/linker/LinkerBlock.hpp>
#include <fma/Project.hpp>
#include <iostream>

using namespace FMA;
using namespace FMA::memory;
using namespace FMA::instruct;
using namespace FMA::assem;
using namespace FMA::symbol;

// ----------------------------------------------------------------------------
MemoryBlock::MemoryBlock(MemoryAdapter *adapter, MemoryBlock::Usage usage, const std::string &name)
  : FMA::plugin::MemoryBlock(usage)
  , nameHint(name)
  , adapter(adapter)
  , _location(new MemoryLocationList())
  , linkerBlock(NULL)
  , isUsed(true)
{
}

// ----------------------------------------------------------------------------
MemoryBlock::~MemoryBlock() {
  clear();
}

// ----------------------------------------------------------------------------
void MemoryBlock::clear() {
  for (auto &instruction : instructions) {
    Instruction *inst = instruction;
    delete inst;
  }

  instructions.clear();
}

// ----------------------------------------------------------------------------
void MemoryBlock::setNameHint(const std::string &name) {
  nameHint = name;
}

// ----------------------------------------------------------------------------
SymbolReferencePtr MemoryBlock::createReference() {
  if (!selfReference) {
    selfReference = adapter->getSymbols()->createReference(nameHint);
  }

  return selfReference;
}

// ----------------------------------------------------------------------------
void MemoryBlock::write(void *data, uint32_t size) {
  if (!instructions.size() || !instructions.back()->containsBinaryData()) {
    write(new instruct::BinaryData(data, size));
    return;
  }

  instructions.back()->appendBinaryData(data, size);
}

// ----------------------------------------------------------------------------
void MemoryBlock::write(const ReferencePtr &reference, uint32_t size)  {
  write(new instruct::DynamicBinaryData(reference, size));
}

// ----------------------------------------------------------------------------
void MemoryBlock::write(Instruction *instruct) {
  instructions.push_back(instruct);
}

// ----------------------------------------------------------------------------
ReferencePtr MemoryBlock::reference() {
  return reference("__ref");
}

// ----------------------------------------------------------------------------
ReferencePtr MemoryBlock::reference(const std::string &name) {
  std::cout << "TODO: Reference current offset " << name << std::endl;

  return ReferencePtr();
}

// ----------------------------------------------------------------------------
ReferencePtr MemoryBlock::reference(const symbol::SymbolReferencePtr &reference) {
  write(new assem::LabelInstruction(reference->getName()));

  return reference;
}

// ----------------------------------------------------------------------------
bool MemoryBlock::buildByteCode(BinaryCodeGeneratorScope *scope) {
  for (auto &instruction : instructions) {
    if (!buildByteCode(scope, instruction)) {
      return false;
    }
  }

  return true;
}

// ----------------------------------------------------------------------------
bool MemoryBlock::buildByteCode(BinaryCodeGeneratorScope *scope, Instruction *instruct) {
  std::string identifier = instruct->getIdentifier();
  Project *project = scope->getProject();

  linkerBlock = scope->getLinkerBlock();
  linkerBlock->setNameHint(nameHint);

  if (buildByteCode(scope, instruct, identifier)) {
    // no op
  } else if (identifier == "$DATA") {
    BinaryData *data = dynamic_cast<BinaryData*>(instruct);
    scope->getLinkerBlock()->write(data->getData(), data->getSize());
  } else if (identifier == "$SYM") {
    DynamicBinaryData *data = dynamic_cast<DynamicBinaryData*>(instruct);
    scope->getLinkerBlock()->write(data->getData(), data->getSize());
  } else {
    project->log().error() << "Unable to build bytecode for instruction " << identifier;
    return false;
  }

  return true;
}

// ----------------------------------------------------------------------------
bool MemoryBlock::buildByteCode(assem::BinaryCodeGeneratorScope *scope, assem::Instruction *instruct, const std::string &name) {
  Project *project = scope->getProject();

  for (auto &generator : project->getByteCodeGenerators()) {
    if (generator->supports(name) && generator->generate(scope, instruct, name)) {
      return true;
    }
  }

  return false;
}

// ----------------------------------------------------------------------------
plugin::MemoryLocationPtr MemoryBlock::location() {
  return std::dynamic_pointer_cast<plugin::MemoryLocation>(_location);
}

// ----------------------------------------------------------------------------
