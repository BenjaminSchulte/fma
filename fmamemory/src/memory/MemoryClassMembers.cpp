#include <memory/MemoryClassMembers.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/types/Class.hpp>
#include <fma/core/Number.hpp>
#include <iostream>

using namespace FMA;
using namespace FMA::memory;
using namespace FMA::types;
using namespace FMA::interpret;
using namespace FMA::core;

namespace FMA {
namespace types {
  template<> const char *getInternalObjectName<MemoryClassMembersPtr>() { return "MemoryClassMembers"; }
}
}


// ----------------------------------------------------------------------------
MemoryClassMembers::MemoryClassMembers(const ClassPtr &klass)
  : klass(klass)
{
}

// ----------------------------------------------------------------------------
MemoryClassMembers::~MemoryClassMembers() {
  removeAllMembers();
}

// ----------------------------------------------------------------------------
void MemoryClassMembers::removeAllMembers() {
  for (auto &ptr : memberList) {
    MemoryClassMemberItem *item = ptr;
    delete item;
  }
  memberList.clear();
  memberMap.clear();
}

// ----------------------------------------------------------------------------
void MemoryClassMembers::declare(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter, const std::string &name) {
  MemoryClassMemberItem *item = new MemoryClassMemberItem(context, parameter, name);
  memberList.push_back(item);
  memberMap[name] = item;
}

// ----------------------------------------------------------------------------
MemoryClassMembersPtr MemoryClassMembers::getClassMembers(const ClassPtr &klass) {
  TypePtr member = klass->getOwnMember("@__memory_class_members");
  if (member->isInternalObjectOfType("MemoryClassMembers")) {
    return dynamic_cast<InternalMemoryClassMembersValue*>(member.get())->getValue();
  }

  member = TypePtr(new InternalMemoryClassMembersValue(MemoryClassMembersPtr(new MemoryClassMembers(klass))));
  klass->setMember("@__memory_class_members", member);

  klass->setMember("size", TypePtr(new InternalFunctionValue("size", MemoryClassMembers::size)));
  klass->setMember("offset_of?", TypePtr(new InternalFunctionValue("offset_of?", MemoryClassMembers::offset_of_optional)));
  klass->setMember("offset_of", TypePtr(new InternalFunctionValue("offset_of", MemoryClassMembers::offset_of_force)));

  return dynamic_cast<InternalMemoryClassMembersValue*>(member.get())->getValue();
}

// ----------------------------------------------------------------------------
uint64_t MemoryClassMembers::getLocalSizeOnly() {
  uint64_t size = 0;
  for (auto &item : memberList) {
    size += item->size();
  }

  return size;
}

// ----------------------------------------------------------------------------
uint64_t MemoryClassMembers::getSize() {
  return getSizeOfClass(klass);
}

// ----------------------------------------------------------------------------
uint64_t MemoryClassMembers::getSizeOfClass(const types::ClassPtr &klass) {
  uint64_t size = 0;

  if (klass->hasOwnMember("@__memory_class_members")) {
    size += getClassMembers(klass)->getLocalSizeOnly();
  }

  for (const ClassPtr &parent : klass->getParents()) {
    size += getSizeOfClass(parent);
  }

  return size;
}

// ----------------------------------------------------------------------------
MemoryClassMemberItem* MemoryClassMembers::getLocalMember(const std::string &name) {
  for (auto &item : memberList) {
    if (item->getName() == name) {
      return item;
    }
  }

  return nullptr;
}

// ----------------------------------------------------------------------------
MemoryClassMemberItem* MemoryClassMembers::getMember(const std::string &name) {
  return getMemberInClass(klass, name);
}

// ----------------------------------------------------------------------------
MemoryClassMemberItem* MemoryClassMembers::getMemberInClass(const types::ClassPtr &klass, const std::string &name) {
  if (klass->hasOwnMember("@__memory_class_members")) {
    MemoryClassMemberItem *result = getClassMembers(klass)->getLocalMember(name);
    
    if (result) {
      return result;
    }
  }

  for (const ClassPtr &parent : klass->getParents()) {
    MemoryClassMemberItem *result = getMemberInClass(parent, name);
    if (result) {
      return result;
    }
  }

  return nullptr;
}

// ----------------------------------------------------------------------------
int64_t MemoryClassMembers::getOffsetOf(const std::string &name) {
  return getOffsetOfInClass(klass, name);
}

// ----------------------------------------------------------------------------
int64_t MemoryClassMembers::getLocalOffsetOf(const std::string &name) {
  int64_t size = 0;
  for (auto &item : memberList) {
    if (item->getName() == name) {
      return size;
    }

    size += item->size();
  }

  return -1;
}

// ----------------------------------------------------------------------------
int64_t MemoryClassMembers::getOffsetOfInClass(const types::ClassPtr &klass, const std::string &name) {
  int64_t size=0, offset;

  for (const ClassPtr &parent : klass->getParents()) {
    offset = getOffsetOfInClass(parent, name);
    
    if (offset == -1) {
      size += getSizeOfClass(parent);
    } else {
      return offset + size;
    }
  }
  
  if (klass->hasOwnMember("@__memory_class_members")) {
    offset = getClassMembers(klass)->getLocalOffsetOf(name);

    if (offset != -1) {
      return size + offset;
    }
  }
  
  return -1;
}

// ----------------------------------------------------------------------------
TypePtr MemoryClassMembers::getTypeHintOf(const std::string &name) {
  auto *result = getMember(name);
  if (!result) {
    return TypePtr();
  }

  return result->getTypeHint();
}

// ----------------------------------------------------------------------------
ResultPtr MemoryClassMembers::size(const ContextPtr &context, const GroupedParameterList &) {
  MemoryClassMembersPtr members = getClassMembers(context->self()->asClass());

  return NumberClass::createInstance(context, members->getSize());
}

// ----------------------------------------------------------------------------
ResultPtr MemoryClassMembers::offset_of_optional(const ContextPtr &context, const GroupedParameterList &params) {
  const TypeList &args = params.only_args();
  if (!args.size()) {
    return ResultPtr(new Result());
  }

  MemoryClassMembersPtr members = getClassMembers(context->self()->asClass());
  std::string name = args.front()->convertToString(context);

  return NumberClass::createInstance(context, members->getOffsetOf(name));
}

// ----------------------------------------------------------------------------
ResultPtr MemoryClassMembers::offset_of_force(const ContextPtr &context, const GroupedParameterList &params) {
  const TypeList &args = params.only_args();
  if (!args.size()) {
    return ResultPtr(new Result());
  }

  MemoryClassMembersPtr members = getClassMembers(context->self()->asClass());
  std::string name = args.front()->convertToString(context);
  if (members->getMember(name) == NULL) {
    context->log().error() << "Member '" << name << "' does not exist on " << context->self()->asClass()->getName();
    return ResultPtr(new Result());
  }

  return NumberClass::createInstance(context, members->getOffsetOf(name));
}

// ----------------------------------------------------------------------------
