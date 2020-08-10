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
  TypePtr member = klass->getMember("@__memory_class_members");
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
uint64_t MemoryClassMembers::getSize() {
  uint64_t size = 0;
  for (auto &item : memberList) {
    size += item->size();
  }
  return size;
}

// ----------------------------------------------------------------------------
MemoryClassMemberItem* MemoryClassMembers::getMember(const std::string &name) {
  for (auto &item : memberList) {
    if (item->getName() == name) {
      return item;
    }
  }

  return NULL;
}

// ----------------------------------------------------------------------------
uint64_t MemoryClassMembers::getOffsetOf(const std::string &name) {
  uint64_t size = 0;
  for (auto &item : memberList) {
    if (item->getName() == name) {
      return size;
    }

    size += item->size();
  }

  return 0;
}

// ----------------------------------------------------------------------------
TypePtr MemoryClassMembers::getTypeHintOf(const std::string &name) {
  for (auto &item : memberList) {
    if (item->getName() == name) {
      return item->getTypeHint();
    }
  }

  return TypePtr();
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
