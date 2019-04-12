#ifndef __FMA_CORE_SYMBOLREFERENCE_H__
#define __FMA_CORE_SYMBOLREFERENCE_H__

#include <fma/types/Class.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/symbol/CalculatedNumber.hpp>

namespace FMA {
namespace symbol {
  typedef std::shared_ptr<class Reference> ReferencePtr;
}
namespace core {

class SymbolReferenceClass : public types::Class {
public:
  static types::ClassPtr create(const types::RootModulePtr &root, const types::ClassPtr &Class);
  static interpret::ResultPtr createInstance(const interpret::ContextPtr &context, const symbol::ReferencePtr &reference);

  // PROTOTYPE
  static interpret::ResultPtr initialize(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr to_sym(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr to_b(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr to_s(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  static interpret::ResultPtr op_add(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr op_sub(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr op_div(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr op_mul(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr op_rem(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr op_and(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr op_or(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr op_xor(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr op_lshift(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr op_rshift(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  static interpret::ResultPtr calculated(symbol::CalculatedNumber::Operand op, const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  static const symbol::ReferencePtr &reference(const interpret::ContextPtr &context);
  static const symbol::ReferencePtr &reference(const types::TypePtr &value);
};

}
}

#endif
