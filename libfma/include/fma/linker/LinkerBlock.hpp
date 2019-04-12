#ifndef __FMA_LINKER_LINKERBLOCK_H__
#define __FMA_LINKER_LINKERBLOCK_H__

#include <cstdint>
#include <vector>
#include <fma/symbol/Reference.hpp>
#include <fma/assem/Operand.hpp>
#include <fma/plugin/MemoryPluginAdapter.hpp>

namespace FMA {
namespace linker {

struct LinkerBlockReference {
  LinkerBlockReference() : reference(), offset(0), size(0), resolved(true) {}
  LinkerBlockReference(const symbol::ReferencePtr &reference, uint32_t offset, uint32_t size)
    : reference(reference), offset(offset), size(size), resolved(false) {}

  symbol::ReferencePtr reference;
  uint32_t offset;
  uint32_t size;
  bool resolved;
};
typedef std::vector<LinkerBlockReference> LinkerBlockReferenceList;

struct LinkerBlockSymbol {
  LinkerBlockSymbol() : name(), offset(0) {}
  LinkerBlockSymbol(const std::string &name, uint32_t offset)
    : name(name), offset(offset) {}

  std::string name;
  uint32_t offset;
};
typedef std::vector<LinkerBlockSymbol> LinkerBlockSymbolList;

class LinkerBlock {
public:
  LinkerBlock();
  virtual ~LinkerBlock();

  void write(const void *data, uint32_t size);
  void replace(const void *data, uint32_t offset, uint32_t size);
  inline void writeNumber(uint64_t number, uint32_t size) { write(&number, size); }
  void write(const assem::Operand *operand, uint32_t size);
  void write(const symbol::ReferencePtr &reference, uint32_t size);
  void symbol(const std::string &name);
  symbol::ReferencePtr privateSymbol(plugin::MemorySymbolMap *map);

  bool calculateSymbols(Project *, const plugin::MemorySymbolMap *);

  inline void setNameHint(const std::string &_name) { name=_name; }
  inline const std::string &getNameHint() const { return name; }

  inline bool hasLocation() const { return !!_location; }
  inline void setLocation(const plugin::MemoryLocationPtr &location) {
    _location = location;
  }
  inline const plugin::MemoryLocationPtr &location() const { return _location; }

  inline void *getData() const { return data; }
  inline uint32_t getSize() const { return size; }
  inline const LinkerBlockReferenceList &getReferences() const { return references; }
  inline const LinkerBlockSymbolList &getSymbols() const { return symbols; }

  inline void setPlacement(const plugin::MemoryBlockPlacement *_placement) { placement = _placement; }
  inline const plugin::MemoryBlockPlacement *getPlacement() const { return placement; }

protected:
  void allocate(uint32_t size);

  void *data;
  uint32_t size;
  uint32_t memorySize;
  std::string name;

  LinkerBlockReferenceList references;
  LinkerBlockSymbolList symbols;

  plugin::MemoryLocationPtr _location;
  const plugin::MemoryBlockPlacement *placement;
};

}
}

#endif
