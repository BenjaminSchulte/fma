#ifndef __FMA_TYPES_CLASS_H__
#define __FMA_TYPES_CLASS_H__

#include <vector>
#include "Base.hpp"

namespace FMA {
namespace interpret {
  class BaseContext;
  typedef std::shared_ptr<BaseContext> ContextPtr;

  struct Parameter;
  struct GroupedParameterList;
}
namespace types {
class ClassPrototype;

class Class : public Base {
public:
  Class(const std::string &name, const std::string &fullName);

  virtual bool isClass() const { return true; }
  virtual ClassPtr asClass() { return std::dynamic_pointer_cast<Class>(getPointer()); }

  ObjectPtr createInstance(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameters);

  void extends(const ClassPtr &other);
  virtual std::string asString() const;
  virtual void dump(const std::string &);

  const std::string &getName() const { return name; }
  const std::string &getFullName() const { return fullName; }
  const ClassPrototypePtr &getPrototype();

  virtual bool hasMember(const std::string &name) const;
  virtual TypePtr getMember(const std::string &name) const;

  virtual bool hasPrototypeMember(const std::string &name);
  virtual TypePtr getPrototypeMember(const std::string &name);

  bool isInstanceOf(const ClassPtr &other) const;
  bool isInstanceOf(const std::string &other) const;

  virtual interpret::ResultPtr callWithoutDecoratorTest(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter); 
  inline const ClassPtr &getParent() const { return parent; };

protected:
  std::string name;
  std::string fullName;
  mutable ClassPrototypePtr prototype;
  ClassPtr parent;
};

}
}

#endif
