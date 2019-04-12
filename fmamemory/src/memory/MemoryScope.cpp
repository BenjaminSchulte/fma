#include <memory/MemoryScope.hpp>

using namespace FMA::memory;

// ----------------------------------------------------------------------------
MemoryScope::MemoryScope(MemoryMap *map)
  : MemoryAllocation(map)
{
}

// ----------------------------------------------------------------------------
MemoryScope::~MemoryScope() {
}

// ----------------------------------------------------------------------------
MemoryScopePtr MemoryScope::createScope() {
  MemoryScopePtr scope(new MemoryScope(map));
  addChild(scope);
  return scope;
}

// ----------------------------------------------------------------------------
void MemoryScope::addChild(const MemoryAllocationPtr &child) {
  _children.push_back(child);
  child->addParent(getScopePointer());
}

// ----------------------------------------------------------------------------
void MemoryScope::dump(const std::string &prefix) {
  MemoryAllocation::dump(prefix);

  for (auto &decl : _children) {
    decl->dump(prefix + "  ");
  }
}

// ----------------------------------------------------------------------------
void MemoryScope::writeSymbolReferences(class MemorySymbolMap *symbols) {
  MemoryAllocation::writeSymbolReferences(symbols);

  for (auto &decl : _children) {
    decl->writeSymbolReferences(symbols);
  }
}

// ----------------------------------------------------------------------------
