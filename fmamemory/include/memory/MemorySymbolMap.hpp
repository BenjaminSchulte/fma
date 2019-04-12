#ifndef __FMA_MEMORY_SYMBOLMAP_H__
#define __FMA_MEMORY_SYMBOLMAP_H__

#include <string>
#include <memory>
#include <map>
#include <fma/plugin/MemoryPluginAdapter.hpp>
#include "MemoryPlacement.hpp"

namespace FMA {
namespace symbol {
typedef std::shared_ptr<class SymbolReference> SymbolReferencePtr;
}
namespace memory {
typedef std::map<std::string, symbol::SymbolReferencePtr> SymbolMap;
typedef std::map<std::string, MemoryPlacement> PlacementMap;

class MemorySymbolMap : public plugin::MemorySymbolMap {
public:
  symbol::SymbolReferencePtr createReference();
  symbol::SymbolReferencePtr createReference(const std::string &hint);

  void resolve(const symbol::SymbolReferencePtr &symbol, const MemoryPlacement &placement);
  void resolve(const std::string &symbol, const MemoryPlacement &placement);

  bool hasResolved(const std::string &) const;
  uint64_t getResolved(const std::string &) const;

  std::vector<std::string> getSymbolNames() const;

  virtual symbol::ReferencePtr createCommand(const std::string &command);
  virtual const std::vector<plugin::MemorySymbolMapCommand> &getCommands() const;

  void dump() const;

protected:
  SymbolMap symbols;
  PlacementMap resolvedSymbols;
  std::vector<plugin::MemorySymbolMapCommand> commands;
};

}
}

#endif
