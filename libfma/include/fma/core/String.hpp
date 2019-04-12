#ifndef __FMA_CORE_STRING_H__
#define __FMA_CORE_STRING_H__

#include <string>
#include <memory>
#include <fma/types/Class.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/serialize/SerializeObject.hpp>
#include <fma/serialize/DeserializeObject.hpp>

namespace FMA {
namespace interpret {
class BaseContext;
typedef std::shared_ptr<BaseContext> ContextPtr;
}
namespace core {

class StringClass : public types::Class {
public:
  static types::ClassPtr create(const types::RootModulePtr &root, const types::ClassPtr &Class);
  static interpret::ResultPtr createInstance(const interpret::ContextPtr &context, const std::string &value);

  // STATIC

  // PROTOTYPE
  static interpret::ResultPtr append(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr bytes(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr downcase(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr initialize(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr length(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr empty_qm(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr prepend(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr to_n(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr to_s(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr upcase(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr sha256(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  static interpret::ResultPtr op_eq(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  static interpret::ResultPtr op_add(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr op_mul(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  // SERIALIZE
  static void serializeObject(const types::TypePtr &type, const serialize::SerializeObjectPtr &);
  static bool deserializeObject(const serialize::DeserializeObjectPtr &);
};

}
}

#endif
