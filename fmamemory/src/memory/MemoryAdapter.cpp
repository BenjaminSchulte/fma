#include <memory/MemoryAdapter.hpp>
#include <memory/MemoryBlock.hpp>
#include <memory/MemorySymbolMap.hpp>
#include <memory/MemoryClassMembers.hpp>
#include <fma/types/Class.hpp>
#include <fma/types/Object.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/Project.hpp>
#include <iostream>

using namespace FMA;
using namespace FMA::memory;
using namespace FMA::interpret;
using namespace FMA::symbol;
using namespace FMA::types;
using namespace FMA::linker;

// ----------------------------------------------------------------------------
MemoryAdapter::MemoryAdapter(Project *project)
  : FMA::plugin::MemoryPluginAdapter(project)
  , symbols(new MemorySymbolMap())
{
}

// ----------------------------------------------------------------------------
MemoryAdapter::~MemoryAdapter() {
  removeAllBlocks();
  removeAllMemoryMaps();
  removeAllPlacements();
  delete symbols;
}

// ----------------------------------------------------------------------------
void MemoryAdapter::removeAllBlocks() {
  while (blocks.size()) {
    MemoryBlock *block = blocks.back();
    blocks.pop_back();
    delete block;
  }
}

// ----------------------------------------------------------------------------
void MemoryAdapter::removeAllMemoryMaps() {
  while (blocks.size()) {
    MemoryMap *block = memoryMaps.back();
    blocks.pop_back();
    delete block;
  }
}

// ----------------------------------------------------------------------------
void MemoryAdapter::removeAllPlacements() {
  for (auto &item : placements) {
    MemoryPlacement *placement = item;
    delete placement;
  }
  placements.clear();
}

// ----------------------------------------------------------------------------
FMA::plugin::MemoryBlock *MemoryAdapter::allocateBlock(MemoryBlock::Usage usage, const std::string &name) {
  MemoryBlock *block = new MemoryBlock(this, usage, name);
  blocks.push_back(block);
  return block;
}

// ----------------------------------------------------------------------------
MemoryMap *MemoryAdapter::createMemoryMap() {
  MemoryMap *map = new MemoryMap(this);
  memoryMaps.push_back(map);
  return map;
}

// ----------------------------------------------------------------------------
const plugin::MemoryBlockList &MemoryAdapter::getBlocks() {
  _result.clear();
  for (auto &block : blocks) {
    if (block->shouldBeLinked()) {
      _result.push_back(block);
    }
  }
  return _result;
}

// ----------------------------------------------------------------------------
SymbolReferencePtr MemoryAdapter::createReference(const std::string &name) {
  return getSymbols()->createReference(name);
}

// ----------------------------------------------------------------------------
ResultPtr MemoryAdapter::createScope(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (args.size() != 1) {
    context->log().error() << "Invalid number of arguments for 'scope'";
    return ResultPtr(new Result());
  }

  std::string name = args.front()->convertToString(context);
  if (!context->hasCallee()) {
    context->log().error() << "Call to 'scope' requires valid callee";
    return ResultPtr(new Result());
  }

  ClassPtr scopeClass = context->getRootLevelContext()->resolve("MemoryScope")->asClass();
  if (!scopeClass) {
    return ResultPtr(new Result());
  }

  TypePtr type = scopeClass->createInstance(context, parameter);
  context->getCallee()->setMember(name, type);
  return ResultPtr(new Result(context, type));
}

// ----------------------------------------------------------------------------
ResultPtr MemoryAdapter::createDeclaration(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (args.size() != 1) {
    context->log().error() << "Invalid number of arguments for 'declare'";
    return ResultPtr(new Result());
  }

  std::string name = args.front()->convertToString(context);
  if (!context->hasCallee()) {
    context->log().error() << "Call to 'declare' requires valid callee";
    return ResultPtr(new Result());
  }

  if (context->getCallee()->isClass()) {
    return createClassMemberDeclaration(context, parameter, name);
  } else {
    return createGlobalDeclaration(context, parameter, name);
  }
}

// ----------------------------------------------------------------------------
ResultPtr MemoryAdapter::createClassMemberDeclaration(const ContextPtr &context, const GroupedParameterList &parameter, const std::string &name) {
  MemoryClassMembersPtr members = MemoryClassMembers::getClassMembers(context->getCallee()->asClass());

  members->declare(context, parameter, name);

  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
ResultPtr MemoryAdapter::createGlobalDeclaration(const ContextPtr &context, const GroupedParameterList &parameter, const std::string &name) {
  ClassPtr scopeClass = context->getRootLevelContext()->resolve("MemoryVariable")->asClass();
  if (!scopeClass) {
    return ResultPtr(new Result());
  }

  TypePtr type = scopeClass->createInstance(context, parameter);
  context->getCallee()->setMember(name, type);
  return ResultPtr(new Result(context, type));
}

// ----------------------------------------------------------------------------
bool MemoryAdapter::placeDynamicBlocks() {
  for (auto &map : dynamicMemoryMaps) {
    if (!map->place()) {
      return false;
    }
  }
  
  return true;
}

// ----------------------------------------------------------------------------
bool MemoryAdapter::placeStaticBlocks() {
  for (auto &map : staticMemoryMaps) {
    if (!map->place()) {
      return false;
    }
  }

  return true;
}

// ----------------------------------------------------------------------------
MemoryMap *MemoryAdapter::getStaticMemoryMap() {
  if (!staticMemoryMaps.size()) {
    project->log().error() << "Could not find memory map for static code";
    return NULL;
  }

  return staticMemoryMaps.front();
}

// ----------------------------------------------------------------------------
plugin::MemorySymbolMap *MemoryAdapter::getSymbolMap() const {
  return symbols;
}

// ----------------------------------------------------------------------------
bool MemoryAdapter::registerLinkerBlockSymbols(LinkerBlock *block) {
  const MemoryPlacement *placement = dynamic_cast<const MemoryPlacement*>(block->getPlacement());
  if (!placement->isValid()) {
    for (const auto &symbol : block->getSymbols()) {
      project->log().warn() << "Skipping invalid symbol: " << symbol.name;
    }

    return true;
  }

  for (const auto &symbol : block->getSymbols()) {
    auto item = placement->withOffset(symbol.offset, 0);

    if (symbol.offset == 0) {
      getSymbols()->resolve(symbol.name, item, "CODE", block->getSize());
    } else {
      getSymbols()->resolve(symbol.name, item, "LABEL");
    }
  }

  return true;
}

// ----------------------------------------------------------------------------
const plugin::MemoryBlockPlacement *MemoryAdapter::requireStaticBlockPlacement(LinkerBlock *block) {
  MemoryMap *map = getStaticMemoryMap();
  if (!map) {
    return NULL;
  }

  MemoryScopePtr scope = map->createMemoryScope();
  scope->setNameHint(block->getNameHint());
  scope->setSize(block->getSize());

  if (block->hasLocation()) {
    scope->copyLocationFrom(*std::dynamic_pointer_cast<MemoryLocationList>(block->location()).get());
  }

  return scope->placement();
}

// ----------------------------------------------------------------------------
uint64_t MemoryAdapter::translateAddress(uint64_t address) {
  return getStaticMemoryMap()->translateAddress(address >> 16, address & 0xFFFF);
}

// ----------------------------------------------------------------------------
