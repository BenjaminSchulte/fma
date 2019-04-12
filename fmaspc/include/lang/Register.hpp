#ifndef __FMASPC_LANG_REGISTER_H__
#define __FMASPC_LANG_REGISTER_H__

#include <fma/types/Class.hpp>
#include <fma/types/RootModule.hpp>

namespace FMASPC {
namespace lang {

class RegisterClass : public FMA::types::Class {
public:
  static FMA::types::ClassPtr create(const FMA::types::RootModulePtr &root, const FMA::types::ClassPtr &Class);
  static FMA::interpret::ResultPtr createInstance(const FMA::interpret::ContextPtr &context, const std::string &value);

  // PROTOTYPE
  static FMA::interpret::ResultPtr initialize(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter);
};

}
}

#endif
