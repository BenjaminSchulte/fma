#pragma once

#include <fma/types/Class.hpp>
#include <fma/types/RootModule.hpp>

namespace FMA {
namespace superfx {

class RegisterClass : public FMA::types::Class {
public:
  static FMA::types::ClassPtr create(const FMA::types::RootModulePtr &root, const FMA::types::ClassPtr &Class);
  static FMA::interpret::ResultPtr createInstance(const FMA::interpret::ContextPtr &context, const std::string &value, int index);

  // PROTOTYPE
  static FMA::interpret::ResultPtr initialize(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter);
  static FMA::interpret::ResultPtr size(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter);
  static FMA::interpret::ResultPtr pretend_size(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter);
  
  static FMA::interpret::ResultPtr is_a(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter);
  static FMA::interpret::ResultPtr op_equal(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter);
};

}
}

