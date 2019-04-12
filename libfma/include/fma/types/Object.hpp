#ifndef __FMA_TYPES_OBJECT_H__
#define __FMA_TYPES_OBJECT_H__

#include "Base.hpp"

namespace FMA {
namespace types {

class Object : public Base {
public:
  Object(const ClassPtr &klass);

  virtual bool isObject() const { return true; }
  virtual bool isObjectOfType(const std::string &name) const;
  virtual ObjectPtr asObject() { return std::dynamic_pointer_cast<Object>(getPointer()); }

  virtual std::string asString() const;
  virtual bool hasMember(const std::string &name) const;
  virtual TypePtr getMember(const std::string &name) const;

  virtual interpret::ResultPtr callWithoutDecoratorTest(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter); 
  
  inline const ClassPtr &getClass() const { return klass; }
  virtual void dump(const std::string &prefix);

  virtual serialize::SerializeType getSerializeTypeId() const;
  virtual void serialize(const serialize::SerializeObjectPtr &);

protected:
  ClassPtr klass;
};

}
}

#endif
