#ifndef __FMA_CORE_BOOLEAN_H__
#define __FMA_CORE_BOOLEAN_H__

#include <fma/types/Class.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/serialize/SerializeObject.hpp>
#include <fma/serialize/DeserializeObject.hpp>

namespace FMA {
namespace core {

class BooleanClass : public types::Class {
public:
  static types::ClassPtr create(const types::RootModulePtr &root, const types::ClassPtr &Class);
  static interpret::ResultPtr createInstance(const interpret::ContextPtr &context, bool value);

  // PROTOTYPE
  static interpret::ResultPtr initialize(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  static interpret::ResultPtr op_eq(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  static interpret::ResultPtr to_n(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr to_b(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr to_s(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  static void serializeObject(const types::TypePtr &type, const serialize::SerializeObjectPtr &);
  static bool deserializeObject(const serialize::DeserializeObjectPtr &);
};

}
}

#endif
