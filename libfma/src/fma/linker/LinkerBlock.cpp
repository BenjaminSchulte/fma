#include <fma/linker/LinkerBlock.hpp>
#include <fma/symbol/SymbolReference.hpp>
#include <fma/symbol/CalculatedNumber.hpp>
#include <fma/symbol/ConstantNumber.hpp>
#include <fma/Project.hpp>
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace FMA::linker;
using namespace FMA::symbol;

// ----------------------------------------------------------------------------
LinkerBlock::LinkerBlock() : size(0), memorySize(256) {
  data = malloc(memorySize);
}

// ----------------------------------------------------------------------------
LinkerBlock::~LinkerBlock() {
  free(data);
}

// ----------------------------------------------------------------------------
void LinkerBlock::allocate(uint32_t _size) {
  uint32_t newSize = size + _size;
  if (newSize <= memorySize) {
    return;
  }

  memorySize = newSize + 256;

  void *newData = malloc(memorySize);
  memcpy(newData, data, size);
  free(data);
  data = newData;
}

// ----------------------------------------------------------------------------
void LinkerBlock::write(const void *_data, uint32_t _size) {
  if (!_size) {
    return;
  }

  allocate(_size);

  memcpy((char*)data + size, _data, _size);
  size += _size;
/*
  const uint8_t *__data = reinterpret_cast<const uint8_t*>(_data);
  for (uint32_t i=0; i<_size; i++) {
    std::cout << "| " << std::hex << (int)__data[i] << std::endl;
  }
//*/
}

// ----------------------------------------------------------------------------
void LinkerBlock::write(const LinkerBlock *other) {
  uint32_t oldSize = size;

  write(other->data, other->size);
  
  for (LinkerBlockReference reference : other->references) {
    reference.offset += oldSize;
    references.push_back(reference);
  }

  for (LinkerBlockSymbol symbol : other->symbols) {
    symbol.offset += oldSize;
    symbols.push_back(symbol);
  }
}

// ----------------------------------------------------------------------------
void LinkerBlock::write(const ReferencePtr &reference, uint32_t _size) {
  if (reference->isConstant()) {
    writeNumber(reference->asConstant(), _size);
    return;
  }

  references.push_back(LinkerBlockReference(reference, size, _size));

  allocate(_size);
  size += _size;
/*
  for (uint32_t i=0; i<_size; i++) {
    std::cout << "| ??" << " " << reference->asString() << std::endl;
  }
//*/
}

// ----------------------------------------------------------------------------
void LinkerBlock::write(const assem::Operand *operand, uint32_t size) {
  if (operand->isConstant()) {
    writeNumber(operand->asConstant(), size);
    return;
  }

  if (operand->isResolvedAddress()) {
    writeNumber(operand->asResolvedAddress(), size);
    return;
  }

  if (operand->isSymbolReference()) {
    ReferencePtr ref = operand->asSymbolReference();
    write(ref, size);
    return;
  }

  std::cerr << "Unable to write operand " << operand->asString() << std::endl;
}

// ----------------------------------------------------------------------------
void LinkerBlock::replace(const void *newData, uint32_t offset, uint32_t size) {
  if (offset + size > memorySize) {
    return;
  }

  memcpy((char*)data + offset, newData, size);
}

// ----------------------------------------------------------------------------
void LinkerBlock::symbol(const std::string &name) {
  symbols.push_back(LinkerBlockSymbol(name, size));
}

// ----------------------------------------------------------------------------
ReferencePtr LinkerBlock::privateSymbol(plugin::MemorySymbolMap *map) {
  if (!symbols.size()) {
    SymbolReferencePtr ref = map->createReference(".privateSymbol");
    symbol(ref->getName());
    return ref;
  }
  
  const auto &sym = symbols.front();
  uint64_t relative = size - sym.offset;
  ReferencePtr ref(new CalculatedNumber(
    ReferencePtr(new SymbolReference(sym.name)),
    CalculatedNumber::ADD,
    ReferencePtr(new ConstantNumber(relative))
  ));
  return ref;
}

// ----------------------------------------------------------------------------
bool LinkerBlock::calculateSymbols(Project *project, const plugin::MemorySymbolMap *map) {
  bool valid = true;
  bool success = true;

  for (const auto &reference : references) {
    uint64_t number = reference.reference->resolve(map, valid);
    if (!valid) {
      project->log().error() << "Unable to resolve symbol: " << reference.reference->asString() << " (in " << getNameHint() << ")";
      success = false;
    }

    replace(&number, reference.offset, reference.size);
  }
  
  return success;
}

// ----------------------------------------------------------------------------
