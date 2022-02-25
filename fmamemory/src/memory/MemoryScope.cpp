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
  std::string color = "";

  int len = (prefix.length() / 7) % 6;
  switch (len) {
    case 0: color = "\x1b[31m"; break;
    case 1: color = "\x1b[32m"; break;
    case 2: color = "\x1b[33m"; break;
    case 3: color = "\x1b[34m"; break;
    case 4: color = "\x1b[35m"; break;
    case 5: color = "\x1b[36m"; break;
  }

  if (isShared()) {
    MemoryAllocation::dump(prefix.substr(0, prefix.size() - 1) + "+-" + color + "o \x1b[32m<SHARED> ");
  } else {
    MemoryAllocation::dump(prefix.substr(0, prefix.size() - 1) + "+-" + color + "o ");
  }

  for (unsigned index=0; index<_children.size(); index++) {
    if (index == _children.size() - 1) {
      _children[index]->dump(prefix + color + "  ");
    } else {
      _children[index]->dump(prefix + color + " |");
    }
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
