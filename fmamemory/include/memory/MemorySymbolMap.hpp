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
typedef std::map<std::string, std::string> SymbolTypeMap;
typedef std::map<std::string, uint32_t> SymbolSizeMap;

class MemorySymbolMap : public plugin::MemorySymbolMap {
public:
  symbol::SymbolReferencePtr createReference();
  symbol::SymbolReferencePtr createReference(const std::string &hint);

  void resolve(const symbol::SymbolReferencePtr &symbol, const MemoryPlacement &placement, const std::string &type, uint32_t size=1);
  void resolve(const std::string &symbol, const MemoryPlacement &placement, const std::string &type, uint32_t size=1);

  bool hasResolved(const std::string &) const;
  uint64_t getResolved(const std::string &) const;

  std::string getSymbolTypeHint(const std::string &name) const;
  uint32_t getSymbolSizeHint(const std::string &name) const;

  std::vector<std::string> getSymbolNames() const;

  virtual symbol::ReferencePtr createCommand(const std::string &command);
  void addEmulatorBreakpoint(const symbol::SymbolReferencePtr &) override;
  virtual const std::vector<plugin::MemorySymbolMapCommand> &getCommands() const;
  const std::vector<symbol::SymbolReferencePtr> &getBreakpoints() const override {
    return breakPoints;
  }

  void dump() const;

protected:
  SymbolMap symbols;
  PlacementMap resolvedSymbols;
  SymbolTypeMap symbolTypes;
  SymbolSizeMap symbolSizes;
  std::vector<plugin::MemorySymbolMapCommand> commands;
  std::vector<symbol::SymbolReferencePtr> breakPoints;
};

}
}

#endif
