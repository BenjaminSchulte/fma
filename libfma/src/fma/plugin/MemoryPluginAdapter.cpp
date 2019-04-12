#include <fma/plugin/MemoryPluginAdapter.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/types/InternalValue.hpp>

using namespace FMA::interpret;
using namespace FMA::types;
using namespace FMA::plugin;

namespace FMA {
namespace types {
  template<> const char *getInternalObjectName<MemoryBlock*>() { return "MemoryBlock"; }
}
}

// ----------------------------------------------------------------------------
MemoryBlock *MemoryBlock::getFromContext(const ContextPtr &context) {
  return getFromType(context->self());
}

// ----------------------------------------------------------------------------
MemoryBlock *MemoryBlock::getFromType(const TypePtr &type) {
  TypePtr block = type->getMember("#__memory_block__");
  if (!block->isInternalObjectOfType("MemoryBlock")) {
    return NULL;
  }

  InternalMemoryBlock *value = dynamic_cast<InternalMemoryBlock*>(block.get());
  return value->getValue();
}

// ----------------------------------------------------------------------------
void MemoryBlock::declareInType(const types::TypePtr &type) {
  type->setMember("#__memory_block__", TypePtr(new InternalMemoryBlock(this)));
}

// ----------------------------------------------------------------------------
