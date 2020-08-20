#ifndef __FMA_MEMORY_MEMORYCLASSMEMBERS_H__
#define __FMA_MEMORY_MEMORYCLASSMEMBERS_H__

#include "MemoryClassMemberItem.hpp"

namespace FMA {
namespace memory {

typedef std::shared_ptr<class MemoryClassMembers> MemoryClassMembersPtr;
typedef types::InternalObject<MemoryClassMembersPtr> InternalMemoryClassMembersValue;

typedef std::vector<MemoryClassMemberItem*> MemoryClassMemberItems;
typedef std::map<std::string, MemoryClassMemberItem*> MemoryClassMemberItemMap;

class MemoryClassMembers {
public:
  MemoryClassMembers(const types::ClassPtr &klass);
  virtual ~MemoryClassMembers();

  void removeAllMembers();

  static MemoryClassMembersPtr getClassMembers(const types::ClassPtr &klass);
  uint64_t getLocalSizeOnly();
  uint64_t getSize();
  int64_t getLocalOffsetOf(const std::string &);
  int64_t getOffsetOf(const std::string &);
  MemoryClassMemberItem* getLocalMember(const std::string &);
  MemoryClassMemberItem* getMember(const std::string &);
  types::TypePtr getTypeHintOf(const std::string &);
  static interpret::ResultPtr size(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr offset_of_optional(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr offset_of_force(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  void declare(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter, const std::string &name);

  

protected:
  static uint64_t getSizeOfClass(const types::ClassPtr &klass);
  static int64_t getOffsetOfInClass(const types::ClassPtr &klass, const std::string &);
  static MemoryClassMemberItem* getMemberInClass(const types::ClassPtr &klass, const std::string &);

  types::ClassPtr klass;

  MemoryClassMemberItems memberList;
  MemoryClassMemberItemMap memberMap;
};

}
}

#endif
