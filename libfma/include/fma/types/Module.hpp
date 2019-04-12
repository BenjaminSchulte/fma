#ifndef __FMA_TYPES_MODULE_H__
#define __FMA_TYPES_MODULE_H__

#include "Base.hpp"

namespace FMA {
namespace types {

class Module : public Base {
public:
  Module(const std::string &name);
  
  virtual bool isModule() const { return true; }

  virtual std::string asString() const;
  virtual void dump(const std::string &prefix);

  inline const std::string &getName() const { return name; }

protected:
  std::string name;
};

}
}

#endif
