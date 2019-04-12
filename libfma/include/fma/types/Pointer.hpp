#ifndef __FMA_TYPES_POINTER_H__
#define __FMA_TYPES_POINTER_H__

#include "Base.hpp"
#include <vector>

namespace FMA {
namespace types {

class Pointer : public Base {
public:
  Pointer(const TypePtr &child);
  
  virtual bool isPointer() const { return true; }
  virtual PointerPtr asPointer() { return std::dynamic_pointer_cast<Pointer>(getPointer()); }

  virtual bool hasMember(const std::string &name) const;
  virtual TypePtr getMember(const std::string &name) const;
  
  inline TypePtr getChild() const { return child; }

  virtual std::string asString() const;
protected:
  TypePtr child;
};

}
}

#endif
