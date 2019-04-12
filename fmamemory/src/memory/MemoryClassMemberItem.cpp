#include <memory/MemoryClassMemberItem.hpp>
#include <memory/MemoryVariableClass.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/Project.hpp>
#include <iostream>

using namespace FMA;
using namespace FMA::types;
using namespace FMA::interpret;
using namespace FMA::memory;

// ----------------------------------------------------------------------------
MemoryClassMemberItem::MemoryClassMemberItem(const ContextPtr &context, const GroupedParameterList &parameter, const std::string &name)
  : context(context)
  , parameter(parameter)
  , name(name)
  , initialized(false)
  , itemSize(0)
  , numItems(1)
  , typeHint()
{
}

// ----------------------------------------------------------------------------
MemoryClassMemberItem::~MemoryClassMemberItem() {
}

// ----------------------------------------------------------------------------
void MemoryClassMemberItem::initialize() {
  const TypeMap &kwargs = parameter.only_kwargs();
  TypeMap::const_iterator it;

  initialized = true;

  if ((it = kwargs.find("as")) == kwargs.end()) {
    context->log().error() << "Missing argument 'as' for declaration";
    return;
  }

  const TypePtr &as = it->second;
  if (as->isObjectOfType("String")) {
    itemSize = MemoryVariableClass::getInternalTypeSize(as->convertToString(context));
  }

  it = kwargs.find("size");
  if (it == kwargs.end()) {
    it = kwargs.find("length");
  }
  if (it != kwargs.end()) {
    numItems = it->second->convertToNumber(context);
  }

  if (as->hasMember("__typehint")) {
    typeHint = as->getMember("__typehint");
  }

  if (itemSize == 0) {
    std::cout << "TODO: NESTED CLASS TYPES" << std::endl;
  }
}

// ----------------------------------------------------------------------------
uint64_t MemoryClassMemberItem::size() {
  if (!initialized) {
    initialize();
  }

  return itemSize * numItems;
}
