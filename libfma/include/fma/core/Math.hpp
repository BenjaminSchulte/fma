#ifndef __FMA_CORE_MATH_H__
#define __FMA_CORE_MATH_H__

#include <fma/types/Class.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/serialize/SerializeObject.hpp>
#include <fma/serialize/DeserializeObject.hpp>
#include <fma/Log.hpp>

namespace FMA {
class Project;

namespace core {

class MathClass : public types::Class {
public:
  static types::ClassPtr create(const types::RootModulePtr &root, const types::ClassPtr &Class);

  // STATIC
  static interpret::ResultPtr PI(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr sin(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr cos(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
};

}
}

#endif
