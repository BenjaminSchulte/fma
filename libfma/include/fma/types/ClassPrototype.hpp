#ifndef __FMA_TYPES_CLASSPROTOTYPE_H__
#define __FMA_TYPES_CLASSPROTOTYPE_H__

#include "Base.hpp"

namespace FMA {
namespace types {

class ClassPrototype : public Base {
public:
  ClassPrototype(const ClassPtr &klass) : klass(klass) {}

  virtual bool isClassPrototype() const { return true; }
  virtual ClassPrototypePtr asClassPrototype() { return std::dynamic_pointer_cast<ClassPrototype>(getPointer()); }

  virtual std::string asString() const;

  ClassPtr getClass();

protected:
  std::weak_ptr<Class> klass;
};

}
}

#endif
