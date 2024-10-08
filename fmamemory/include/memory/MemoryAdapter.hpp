#ifndef __FMA_MEMORY_MEMORYADAPTER_H__
#define __FMA_MEMORY_MEMORYADAPTER_H__

#include <fma/plugin/MemoryPluginAdapter.hpp>
#include "MemoryBlock.hpp"
#include "MemoryMap.hpp"

namespace FMA {
namespace memory {

typedef std::map<std::string, class MemoryBlock> MemoryBlockMap;
class MemorySymbolMap;
class MemoryPlacement;

class MemoryAdapter : public FMA::plugin::MemoryPluginAdapter {
public:
  MemoryAdapter(Project *project);
  virtual ~MemoryAdapter();

  FMA::plugin::MemoryBlock *allocateBlock(MemoryBlock::Usage usage, const std::string &name) override;
  inline MemorySymbolMap *getSymbols() const { return symbols; }

  plugin::MemorySymbolMap *getSymbolMap() const override;
  const plugin::MemoryBlockList &getBlocks() override;
  
  symbol::SymbolReferencePtr createReference(const std::string &name) override;
  plugin::MemoryLocationPtr deserializeLocation(Log *, output::DynamicBuffer &) const override;

  interpret::ResultPtr createScope(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  interpret::ResultPtr createDeclaration(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  interpret::ResultPtr createClassMemberDeclaration(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter, const std::string &);
  interpret::ResultPtr createGlobalDeclaration(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter, const std::string &);

  uint64_t translateAddress(uint64_t address) override;
  
  inline void addDynamicMemoryMap(MemoryMap *map) { dynamicMemoryMaps.push_back(map); }
  inline void addStaticMemoryMap(MemoryMap *map) { staticMemoryMaps.push_back(map); }
  bool placeDynamicBlocks() override;
  bool placeStaticBlocks() override;
  bool registerLinkerBlockSymbols(linker::LinkerBlock *block) override;
  const FMA::plugin::MemoryBlockPlacement *requireStaticBlockPlacement(linker::LinkerBlock *block) override;

  MemoryMap *createMemoryMap();
  MemoryMap *getStaticMemoryMap();
  void removeAllMemoryMaps();

  inline Project *getProject() const { return project; }

protected:
  void registerMetaData();

  MemoryBlockList blocks;
  plugin::MemoryBlockList _result;
  MemorySymbolMap *symbols;

  MemoryMapList memoryMaps;
  MemoryMapList staticMemoryMaps;
  MemoryMapList dynamicMemoryMaps;
  std::vector<MemoryPlacement*> placements;

  void removeAllBlocks();
  void removeAllPlacements();
};

}
}

#endif