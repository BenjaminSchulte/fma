#ifndef __FMA_CORE_MAP_H__
#define __FMA_CORE_MAP_H__

#include <fma/types/Class.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/serialize/SerializeObject.hpp>
#include <fma/serialize/DeserializeObject.hpp>

namespace FMA {
namespace core {

typedef std::map<std::string, types::TypePtr> TypeMap;

class MapClass : public types::Class {
public:
  static types::ClassPtr create(const types::RootModulePtr &root, const types::ClassPtr &Class);
  static interpret::ResultPtr createInstance(const interpret::ContextPtr &context, const std::map<std::string, types::TypePtr> &value);

  // PROTOTYPE
  static interpret::ResultPtr initialize(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr each(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr key_qm(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr keys(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr size(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr values_call(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr to_s(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr op_index(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr op_index_set(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  static types::InternalMapValue *internalMap(const interpret::ContextPtr &context);
  static types::InternalMapValue *internalMap(const interpret::ContextPtr &context, const types::TypePtr &self);
  static TypeMap &values(const interpret::ContextPtr &context);

  // SERIALIZE
  static void serializeObject(const types::TypePtr &type, const serialize::SerializeObjectPtr &);
  static bool deserializeObject(const serialize::DeserializeObjectPtr &);
};

}
}

#endif
