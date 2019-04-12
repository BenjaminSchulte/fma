#ifndef __FMA_MEMORY_MEMORYCLASSMEMBERITEM_H__
#define __FMA_MEMORY_MEMORYCLASSMEMBERITEM_H__

#include <fma/types/InternalValue.hpp>
#include <fma/types/Class.hpp>
#include <fma/interpret/ParameterList.hpp>

namespace FMA {
namespace memory {

class MemoryClassMemberItem {
public:
  MemoryClassMemberItem(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter, const std::string &name);
  ~MemoryClassMemberItem();

  uint64_t size();

  inline const std::string &getName() const { return name; }
  inline const types::TypePtr &getTypeHint() const { return typeHint; }

protected:
  interpret::ContextPtr context;
  interpret::GroupedParameterList parameter;
  std::string name;

  bool initialized;
  uint64_t itemSize;
  uint64_t numItems;
  types::TypePtr typeHint;

  void initialize();
};

}
}

#endif
