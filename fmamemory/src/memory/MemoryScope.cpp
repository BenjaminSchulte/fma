#include <memory/MemoryScope.hpp>

using namespace FMA::memory;

// ----------------------------------------------------------------------------
MemoryScope::MemoryScope(MemoryMap *map, bool shared)
  : MemoryAllocation(map)
  , mShared(shared)
{
}

// ----------------------------------------------------------------------------
MemoryScope::~MemoryScope() {
}

// ----------------------------------------------------------------------------
MemoryScopePtr MemoryScope::createScope(bool shared) {
  MemoryScopePtr scope(new MemoryScope(map, shared));
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
