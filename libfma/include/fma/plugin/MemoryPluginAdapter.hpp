#ifndef __FMA_PLUGIN_MEMORYPLUGINADAPTER_H__
#define __FMA_PLUGIN_MEMORYPLUGINADAPTER_H__

#include <memory>
#include <vector>
#include "../types/InternalValue.hpp"

namespace FMA {
class Log;
class Project;
namespace output {
  class DynamicBuffer;
}
namespace linker {
  class LinkerBlock;
}
namespace interpret {
  typedef std::shared_ptr<class BaseContext> ContextPtr; 
}
namespace symbol {
  typedef std::shared_ptr<class Reference> ReferencePtr; 
  typedef std::shared_ptr<class SymbolReference> SymbolReferencePtr; 
}
namespace assem {
  class Instruction; 
  class BinaryCodeGeneratorScope;
}
namespace plugin {

class MemoryBlockPlacement {
public:
  MemoryBlockPlacement() {}
  virtual ~MemoryBlockPlacement() {}

  virtual bool isValid() const = 0;
  virtual uint64_t asTranslatedAddress() const = 0;
  virtual uint64_t asLongAddress() const = 0;
};

struct MemorySymbolMapCommand {
  MemorySymbolMapCommand() {}
  MemorySymbolMapCommand(uint32_t reference, const std::string &command)
    : reference(reference), command(command) {}

  uint32_t reference;
  std::string command;
};

class MemorySymbolMap {
public:
  MemorySymbolMap() {}
  virtual ~MemorySymbolMap() {}

  virtual symbol::SymbolReferencePtr createReference() = 0;
  virtual symbol::SymbolReferencePtr createReference(const std::string &hint) = 0;

  virtual symbol::ReferencePtr createCommand(const std::string &command) = 0;

  virtual bool hasResolved(const std::string &) const = 0;
  virtual uint64_t getResolved(const std::string &) const = 0;

  virtual std::vector<std::string> getSymbolNames() const = 0;
  virtual const std::vector<MemorySymbolMapCommand> &getCommands() const = 0;

  virtual std::string getSymbolTypeHint(const std::string &) const { return "ANY"; }
  virtual uint32_t getSymbolSizeHint(const std::string &) const { return 1; }

  virtual void dump() const {}
};

class MemoryLocation {
public:
  MemoryLocation() {}
  virtual ~MemoryLocation() {}

  virtual void overrideAllow(const interpret::ContextPtr &, const interpret::GroupedParameterList &) = 0;
  virtual void allow(const interpret::ContextPtr &, const interpret::GroupedParameterList &) = 0;
  virtual void deny(const interpret::ContextPtr &, const interpret::GroupedParameterList &) = 0;

  virtual bool serialize(output::DynamicBuffer &) const = 0;
};

typedef std::shared_ptr<MemoryLocation> MemoryLocationPtr;

class MemoryBlock {
public:
  enum Usage {
    READ_ONLY,
    RANDOM_ACCESS
  };

  MemoryBlock(Usage usage) : usage(usage), lastIsReturn(false) {};
  virtual ~MemoryBlock() {}

  virtual void setNameHint(const std::string &) {}
  virtual std::string getNameHint() const { return ""; }
  virtual symbol::SymbolReferencePtr createReference() = 0;

  virtual void markIsUsed(bool used=true) = 0;

  virtual void write(void *data, uint32_t size) = 0;
  virtual void write(const symbol::ReferencePtr &reference, uint32_t size) = 0;
  virtual void write(assem::Instruction *instruct) = 0;
  virtual symbol::ReferencePtr reference() = 0;
  virtual symbol::ReferencePtr reference(const symbol::SymbolReferencePtr &reference) = 0;
  virtual symbol::ReferencePtr reference(const std::string &) { return reference(); }

  virtual bool buildByteCode(assem::BinaryCodeGeneratorScope *scope) = 0;

  virtual MemoryLocationPtr location() = 0;

  static MemoryBlock *getFromContext(const interpret::ContextPtr &context);
  static MemoryBlock *getFromType(const types::TypePtr &type);
  void declareInType(const types::TypePtr &type);

  inline void setLastIsReturn(bool isReturn) { lastIsReturn=isReturn; };
  inline bool isReturned() const { return lastIsReturn; }

protected:
  Usage usage;
  bool lastIsReturn;
};

typedef types::InternalObject<MemoryBlock*> InternalMemoryBlock;
typedef std::vector<MemoryBlock*> MemoryBlockList;

class MemoryPluginAdapter {
public:
  MemoryPluginAdapter(Project *project) : project(project) {}
  virtual ~MemoryPluginAdapter() {}

  virtual MemorySymbolMap *getSymbolMap() const = 0;

  virtual MemoryBlock *allocateBlock(MemoryBlock::Usage usage, const std::string &name) = 0;
  virtual const MemoryBlockList &getBlocks() = 0;  

  virtual MemoryLocationPtr deserializeLocation(Log *, output::DynamicBuffer &) const {
    return MemoryLocationPtr();
  }

  virtual symbol::SymbolReferencePtr createReference(const std::string &name) = 0;

  virtual uint64_t translateAddress(uint64_t address) { return address; }

  virtual bool placeDynamicBlocks() = 0;
  virtual bool placeStaticBlocks() = 0;
  virtual bool registerLinkerBlockSymbols(linker::LinkerBlock *block) = 0;
  virtual const MemoryBlockPlacement *requireStaticBlockPlacement(linker::LinkerBlock *block) = 0;

protected:
  Project *project;
};

}
}

#endif
