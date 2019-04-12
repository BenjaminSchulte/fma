#ifndef __FMA_TYPES_CLASSPROTOTYPE_H__
#define __FMA_TYPES_CLASSPROTOTYPE_H__

#include "Base.hpp"

namespace FMA {
namespace types {

class ClassPrototype : public Base {
public:
  virtual std::string asString() const;
protected:
};

}
}

#endif
