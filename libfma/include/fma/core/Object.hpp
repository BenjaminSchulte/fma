#ifndef __FMA_CORE_OBJECT_H__
#define __FMA_CORE_OBJECT_H__

#include <fma/types/Class.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/serialize/SerializeObject.hpp>
#include <fma/serialize/DeserializeObject.hpp>

namespace FMA {
namespace core {

class ObjectClass : public types::Class {
public:

  static types::ClassPtr create(const types::RootModulePtr &root);

  static interpret::ResultPtr __dump__(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr is_a_qm(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr nil_qm(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr to_s(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr to_b(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr has_attribute_qm(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr respond_to_qm(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr get_class(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  // SERIALIZE
  static void serializeObject(const types::TypePtr &type, const serialize::SerializeObjectPtr &);
  static bool deserializeObject(const serialize::DeserializeObjectPtr &);
};

}
}

#endif
