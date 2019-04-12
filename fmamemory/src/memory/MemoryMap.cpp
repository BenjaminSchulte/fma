#include <memory/MemoryMap.hpp>
#include <memory/MemoryMapBank.hpp>
#include <memory/MemoryAdapter.hpp>
#include <placer/Placer.hpp>
#include <fma/Project.hpp>
#include <iostream>

using namespace FMA::memory;

// ----------------------------------------------------------------------------
MemoryMap::MemoryMap(MemoryAdapter *adapter)
  : adapter(adapter)
  , isDynamic(false)
  , isStatic(false)
{
}

// ----------------------------------------------------------------------------
MemoryMap::~MemoryMap() {
  removeAllBanks();
}

// ----------------------------------------------------------------------------
void MemoryMap::removeAllBanks() {
  for (auto &pair : banks) {
    MemoryMapBank *bank = pair.second;
    delete bank;
  }
  banks.clear();
}

// ----------------------------------------------------------------------------
void MemoryMap::useForDynamicMemory() {
  if (isDynamic) {
    return;
  }

  isDynamic = true;
  adapter->addDynamicMemoryMap(this);
}

// ----------------------------------------------------------------------------
MemoryMapBank *MemoryMap::createBank(MemoryBankIndex index, MemoryBankSize offset, MemoryBankSize size) {
  MemoryMapBank *bank = new MemoryMapBank(this, index, offset, size);
  banks[index] = bank;
  return bank;
}

// ----------------------------------------------------------------------------
MemoryMapShadow *MemoryMap::createShadow(MemoryBankIndex left, MemoryBankSize leftAddress, MemoryBankIndex right, MemoryBankSize rightAddress, MemoryBankSize size) {
  MemoryMapShadow *shadow = new MemoryMapShadow(getBank(left), getBank(right), leftAddress, rightAddress, size);
  shadows.push_back(shadow);

  getBank(left)->addShadow(shadow);
  getBank(right)->addShadow(shadow);
  return shadow;
}

// ----------------------------------------------------------------------------
void MemoryMap::removeAllShadows() {
  for (auto &pair : shadows) {
    MemoryMapShadow *shadow = pair;
    delete shadow;
  }
  shadows.clear();
}

// ----------------------------------------------------------------------------
MemoryMapBank *MemoryMap::getBank(MemoryBankIndex index) {
  MemoryMapBankList::const_iterator it = banks.find(index);
  if (it == banks.end()) {
    return NULL;
  }

  return it->second;
}

// ----------------------------------------------------------------------------
void MemoryMap::dump() {
  std::cout << "| Bnk | Address     | ROM mapping      | Shadow address   |" << std::endl;
  std::cout << "+-----+-------------+------------------+------------------+" << std::endl;
  for (auto &bank : banks) {
    bank.second->dump();
  }
}

// ----------------------------------------------------------------------------
void MemoryMap::dumpScopes() {
  for (auto &scope : _scopes) {
    scope->dump("");
  }
}

// ----------------------------------------------------------------------------
void MemoryMap::useForStaticMemory() {
  if (isStatic) {
    return;
  }

  isStatic = true;
  adapter->addStaticMemoryMap(this);
}

// ----------------------------------------------------------------------------
MemoryScopePtr MemoryMap::createMemoryScope() {
  MemoryScopePtr ptr(new MemoryScope(this));
  _scopes.push_back(ptr);
  return ptr;
}

// ----------------------------------------------------------------------------
MemoryScopePtr MemoryMap::createLooseMemoryScope() {
  return MemoryScopePtr(new MemoryScope(this));
}

// ----------------------------------------------------------------------------
bool MemoryMap::place() {
  placer::Placer placerInstance(adapter->getProject(), this);
  if (!placerInstance.place()) {
    return false;
  }

  for (auto &scope : _scopes) {
    scope->writeSymbolReferences(adapter->getSymbols());
  }

  return true;
}

// ----------------------------------------------------------------------------
uint64_t MemoryMap::translateAddress(MemoryBankIndex index, MemoryBankSize offset) {
  MemoryMapBank *bank = getBank(index);
  if (!bank) {
    adapter->getProject()->log().error() << "Unable to translate address from bank " << (int)index;
    return 0;
  }

  return bank->translateAddress(offset);
}

// ----------------------------------------------------------------------------
