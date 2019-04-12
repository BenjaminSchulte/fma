#ifndef __FMA_INTERPRET_PARAMETER_H__
#define __FMA_INTERPRET_PARAMETER_H__

#include <memory>
#include <string>

namespace FMA {
namespace types {
  typedef std::shared_ptr<class Base> TypePtr;
}
namespace interpret {

struct Parameter {
  enum Type {
    INVALID,
    ARG,
    KWARG,
    BLOCK,
    CONTEXT
  };

  Parameter() : type(INVALID), name(""), value() {}
  Parameter(Type type, const types::TypePtr &value) : type(type), name(""), value(value) {}
  Parameter(const types::TypePtr &value) : type(ARG), name(""), value(value) {}
  Parameter(const std::string &key, const types::TypePtr &value) : type(KWARG), name(key), value(value) {}

  Type type;
  std::string name;
  types::TypePtr value;
};

}
}

#endif
