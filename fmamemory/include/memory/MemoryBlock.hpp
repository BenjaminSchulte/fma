#ifndef __FMA_MEMORY_MEMORYBLOCK_H__
#define __FMA_MEMORY_MEMORYBLOCK_H__

#include <vector>
#include <fma/plugin/MemoryPluginAdapter.hpp>
#include <fma/linker/LinkerBlock.hpp>
#include "MemoryLocationList.hpp"

namespace FMA {
namespace memory {

typedef std::vector<assem::Instruction *> InstructionList;

class MemoryBlock : public FMA::plugin::MemoryBlock {
public:
  MemoryBlock(class MemoryAdapter *adapter, Usage usage, const std::string &name);
  virtual ~MemoryBlock();

  virtual void write(void *data, uint32_t size);
  virtual void write(const symbol::ReferencePtr &reference, uint32_t size);
  virtual void write(assem::Instruction *instruct);
  virtual symbol::ReferencePtr reference();
  virtual symbol::ReferencePtr reference(const symbol::SymbolReferencePtr &reference);
  virtual symbol::ReferencePtr reference(const std::string &name);

  virtual void markIsUsed(bool used=true) { isUsed = used; }
  bool shouldBeLinked() const { return isUsed; }

  virtual bool buildByteCode(assem::BinaryCodeGeneratorScope *scope);
  bool buildByteCode(assem::BinaryCodeGeneratorScope *scope, assem::Instruction *instruct);

  void clear();
  void setNameHint(const std::string &);
  std::string getNameHint() const { return nameHint; }
  symbol::SymbolReferencePtr createReference();

  virtual plugin::MemoryLocationPtr location();

protected:
  std::string nameHint;
  symbol::SymbolReferencePtr selfReference;
  class MemoryAdapter *adapter;
  InstructionList instructions;

  std::shared_ptr<MemoryLocationList> _location;  
  linker::LinkerBlock *linkerBlock;

  bool isUsed;

  bool buildByteCode(assem::BinaryCodeGeneratorScope *scope, assem::Instruction *instruct, const std::string &identifier);
};

typedef std::vector<MemoryBlock*> MemoryBlockList;

}
}

#endif
