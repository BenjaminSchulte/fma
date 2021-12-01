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
  symbol::SymbolReferencePtr createReference() override;
  symbol::SymbolReferencePtr createReference(const std::string &hint) override;

  void resolve(const symbol::SymbolReferencePtr &symbol, const MemoryPlacement &placement, const std::string &type, uint32_t size=1);
  void resolve(const std::string &symbol, const MemoryPlacement &placement, const std::string &type, uint32_t size=1);

  bool hasResolved(const std::string &) const override;
  uint64_t getResolved(const std::string &) const override;

  std::string getSymbolTypeHint(const std::string &name) const override;
  uint32_t getSymbolSizeHint(const std::string &name) const override;

  std::vector<std::string> getSymbolNames() const override;

  virtual symbol::ReferencePtr createCommand(const std::string &command) override;
  void addEmulatorBreakpoint(const symbol::ReferencePtr &, bool notifyOnly, const std::string &comment) override;
  virtual const std::vector<plugin::MemorySymbolMapCommand> &getCommands() const override;
  const std::vector<plugin::MemorySymbolMapBreakpoint> &getBreakpoints() const override {
    return breakPoints;
  }

  void dump() const override;

protected:
  SymbolMap symbols;
  PlacementMap resolvedSymbols;
  SymbolTypeMap symbolTypes;
  SymbolSizeMap symbolSizes;
  std::vector<plugin::MemorySymbolMapCommand> commands;
  std::vector<plugin::MemorySymbolMapBreakpoint> breakPoints;
};

}
}

#endif
