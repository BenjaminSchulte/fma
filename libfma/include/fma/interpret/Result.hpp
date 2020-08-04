#ifndef __FMA_INTERPRET_RESULT_H__
#define __FMA_INTERPRET_RESULT_H__

#include <memory>
#include <string>

#include "../Reference.hpp"

namespace FMA {
namespace types {
typedef std::shared_ptr<class Base> TypePtr;
typedef std::shared_ptr<class Class> ClassPtr;
typedef std::shared_ptr<class Object> ObjectPtr;
}
namespace interpret {

class BaseContext;
typedef std::shared_ptr<BaseContext> ContextPtr;
typedef std::shared_ptr<class Result> ResultPtr;

class Result {
public:
  enum Type {
    STANDARD,
    EARLY_RETURN
  };

  Result(); 
  Result(const ContextPtr &context, const types::TypePtr &value);
  Result(const ContextPtr &context, const types::TypePtr &value, const types::TypePtr &contextObject);

  static ResultPtr executed(const ContextPtr &context, const types::TypePtr &value);
  static ResultPtr returns(const types::TypePtr &value);

  bool isUndefined();
  bool isClass();
  bool isObject();
  bool isModule();
  bool isMacro();

  void dump() const;
  std::string asString() const;

  types::ClassPtr asClass();
  types::ObjectPtr asObject();
  types::ObjectPtr asObjectOfType(const std::string &name);

  const types::TypePtr &get();

  bool hasContextObject() const { return !!contextObject; }
  void withContextObject(const types::TypePtr &object) { contextObject = object; }
  const types::TypePtr &getContextObject() const { return contextObject; }

  inline void makeAssignable(const ContextPtr &owner, const std::string &name) { ownerContext = owner; assignName = name; }
  inline bool isAssignable() const { return !!ownerContext; }
  inline bool isEarlyReturn() const { return type == EARLY_RETURN; }
  void assign(const types::TypePtr &value);

  inline void noEarlyReturn() { type = STANDARD; }

  inline void resolvedAt(const CodeReference &ref) { location=CodeReference(ref); }

  void pretendExecuted();
  void requireExecuted();

protected:
  types::TypePtr value;
  types::TypePtr contextObject;

  Type type;
  bool isExecuted;
  ContextPtr executeContext;

  ContextPtr ownerContext;
  std::string assignName;

  CodeReference location;
};


}
}

#endif
