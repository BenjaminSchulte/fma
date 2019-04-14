#include <boost/io/ios_state.hpp>
#include <memory/MemorySymbolMap.hpp>
#include <memory/MemoryPlacement.hpp>
#include <fma/symbol/SymbolReference.hpp>
#include <fma/symbol/ConstantNumber.hpp>
#include <iostream>
#include <sstream>

using namespace FMA;
using namespace FMA::memory;
using namespace FMA::symbol;

// ----------------------------------------------------------------------------
SymbolReferencePtr MemorySymbolMap::createReference() {
  return createReference(".symbol");
}

// ----------------------------------------------------------------------------
SymbolReferencePtr MemorySymbolMap::createReference(const std::string &hint) {
  if (hint.size() == 0) {
    return createReference();
  }

  if (symbols.find(hint) == symbols.end()) {
    SymbolReferencePtr result(new SymbolReference(hint));
    symbols[hint] = result;
    return result;
  }

  uint64_t counter = 2;
  
  while (true) {
    std::ostringstream os;
    os << hint << "#" << (counter++);
    std::string name = os.str();

    if (symbols.find(name) == symbols.end()) {
      SymbolReferencePtr result(new SymbolReference(name));
      symbols[name] = result;
      return result;
    }
  }

  return SymbolReferencePtr();
}

// ----------------------------------------------------------------------------
void MemorySymbolMap::resolve(const SymbolReferencePtr &symbol, const MemoryPlacement &placement, const std::string &type, uint32_t size) {
  resolve(symbol->getName(), placement, type, size);
}

// ----------------------------------------------------------------------------
void MemorySymbolMap::resolve(const std::string &symbol, const MemoryPlacement &placement, const std::string &type, uint32_t size) {
  resolvedSymbols[symbol] = placement;
  symbolTypes[symbol] = type;
  symbolSizes[symbol] = size;
}

// ----------------------------------------------------------------------------
bool MemorySymbolMap::hasResolved(const std::string &name) const {
  return resolvedSymbols.find(name) != resolvedSymbols.end();
}

// ----------------------------------------------------------------------------
uint64_t MemorySymbolMap::getResolved(const std::string &name) const {
  const auto &it = resolvedSymbols.find(name);
  if (it == resolvedSymbols.end()) {
    return 0;
  }

  return it->second.asLongAddress();
}

// ----------------------------------------------------------------------------
std::string MemorySymbolMap::getSymbolTypeHint(const std::string &name) const {
  const auto &it = symbolTypes.find(name);
  if (it == symbolTypes.end()) {
    return "ANY";
  }

  return it->second;
}

// ----------------------------------------------------------------------------
uint32_t MemorySymbolMap::getSymbolSizeHint(const std::string &name) const {
  const auto &it = symbolSizes.find(name);
  if (it == symbolSizes.end()) {
    return 1;
  }

  return it->second;
}

// ----------------------------------------------------------------------------
std::vector<std::string> MemorySymbolMap::getSymbolNames() const {
  std::vector<std::string> result;

  for (const auto &item : resolvedSymbols) {
    result.push_back(item.first);
  }

  return result;
}

// ----------------------------------------------------------------------------
void MemorySymbolMap::dump() const {
  boost::io::ios_all_saver guard(std::cout);

  for (const auto &item : resolvedSymbols) {
    std::cout << item.first << " = $" << std::hex << item.second.asLongAddress() << std::endl;;
  }
}

// ----------------------------------------------------------------------------
ReferencePtr MemorySymbolMap::createCommand(const std::string &command) {
  uint32_t index = commands.size();
  commands.push_back(plugin::MemorySymbolMapCommand(index, command));
  return ReferencePtr(new ConstantNumber(index));
}

// ----------------------------------------------------------------------------
const std::vector<plugin::MemorySymbolMapCommand> &MemorySymbolMap::getCommands() const {
  return commands;
}

// ----------------------------------------------------------------------------
