#ifndef __FMA_TYPES_BASE_H__
#define __FMA_TYPES_BASE_H__

#include <string>
#include <memory>
#include <vector>
#include <map>
#include "../serialize/SerializeTypes.hpp"

namespace FMA {
namespace serialize {
  typedef std::shared_ptr<class SerializeObject> SerializeObjectPtr;
}
namespace interpret {
  class BaseContext;
  class Result;
  typedef std::shared_ptr<BaseContext> ContextPtr;
  typedef std::shared_ptr<Result> ResultPtr;
  struct GroupedParameterList;
}
namespace types {

enum DecoratorCallType {
  DECORATORCALL_NONE = 0x00,
  DECORATORCALL_INNER = 0x01,
  DECORATORCALL_OUTER = 0x02,
  DECORATORCALL_INNER_AND_OUTER = 0x03
};

typedef std::shared_ptr<class Base> TypePtr;
typedef std::weak_ptr<class Base> WeakTypePtr;
typedef std::shared_ptr<class Macro> MacroPtr;
typedef std::shared_ptr<class Module> ModulePtr;
typedef std::shared_ptr<class RootModule> RootModulePtr;
typedef std::shared_ptr<class Class> ClassPtr;
typedef std::shared_ptr<class ClassPrototype> ClassPrototypePtr;
typedef std::shared_ptr<class Object> ObjectPtr;
typedef std::shared_ptr<class Block> BlockPtr;
typedef std::shared_ptr<class Decorator> DecoratorPtr;
typedef std::shared_ptr<class Decoratable> DecoratablePtr;
typedef std::shared_ptr<class Pointer> PointerPtr;

typedef std::map<std::string, TypePtr> MemberMap;
typedef MemberMap::value_type MemberMapValue;
typedef MemberMap::iterator MemberMapIterator;
typedef MemberMap::const_iterator MemberMapConstIterator;

class Base : public std::enable_shared_from_this<Base> {
public:
  Base();
  virtual ~Base();

  virtual void unload();

  virtual bool isUndefined() const;

  virtual bool isObject() const { return false; }
  virtual ObjectPtr asObject() { return NULL; }
  virtual bool isObjectOfType(const std::string&) const { return false; }

  virtual bool isInternalObject() const { return false; }
  virtual bool isInternalObjectOfType(const std::string &) const { return false; }

  virtual bool isPointer() const { return false; }
  virtual PointerPtr asPointer() { return NULL; }

  virtual bool isClass() const { return false; }
  virtual ClassPtr asClass() { return NULL; }

  virtual bool isClassPrototype() const { return false; }
  virtual ClassPrototypePtr asClassPrototype() { return NULL; }

  virtual bool isMacro() const { return false; }
  virtual MacroPtr asMacro() { return NULL; }
  
  virtual bool isModule() const { return false; }
  virtual ModulePtr asModule() { return NULL; }

  virtual bool isDecoratorContainer() const { return false; }
  virtual bool isDecoratable() const { return false; }
  virtual DecoratorCallType getDecoratorCallType() const { return DECORATORCALL_NONE; }

  virtual bool hasMember(const std::string &name) const;
  virtual TypePtr getDirectMember(const std::string &name) const;
  virtual TypePtr getMember(const std::string &name) const;
  virtual void setMember(const std::string &name, const TypePtr &value);
  virtual void removeMember(const std::string &name);
  virtual const MemberMap &allMembers() const {
    return members;
  }

  virtual std::vector<TypePtr> convertToArray(const interpret::ContextPtr &context);
  virtual std::map<std::string, TypePtr> convertToMap(const interpret::ContextPtr &context);
  virtual std::string convertToString(const interpret::ContextPtr &context);
  virtual long double convertToNumber(const interpret::ContextPtr &context, const std::string &functionName="to_n");
  virtual bool convertToBoolean(const interpret::ContextPtr &context);
  virtual struct InternalRange convertToRange(const interpret::ContextPtr &context);

  virtual interpret::ResultPtr call(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter); 
  virtual interpret::ResultPtr callWithoutDecoratorTest(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter); 
  virtual interpret::ResultPtr callDirect(const std::string &name, const interpret::ContextPtr& context, const interpret::GroupedParameterList &parameter);

  virtual std::string asString() const = 0;

  virtual serialize::SerializeType getSerializeTypeId() const;
  virtual void serialize(const serialize::SerializeObjectPtr &);

  inline std::shared_ptr<Base> getPointer() {
    return shared_from_this();
  }

  virtual void dump(const std::string &prefix);

  inline const std::string &getParentNameHint() const { return parentNameHint; }
  void setParentNameHint(const std::string &hint) { parentNameHint = hint; }

protected:
  MemberMap members;
  std::string parentNameHint;
};

}
}

#endif
