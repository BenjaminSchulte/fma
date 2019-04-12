#ifndef __FMASPC_LANG_PLUGIN_H__
#define __FMASPC_LANG_PLUGIN_H__

#include <fma/plugin/Plugin.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/ParameterList.hpp>

namespace FMASPC {
namespace lang {

class SpcLanguagePlugin : public FMA::plugin::LanguagePlugin {  
public:
  SpcLanguagePlugin(FMA::Project *project);

  const char *getName() const {
    return "SPC language support";
  }

  const char *getDescription() const {
    return "Adds instructions used for SPC development";
  }

  bool initialize();
  void release();

  static FMA::interpret::ResultPtr dp(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter);
  static FMA::interpret::ResultPtr addr(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter);
  static FMA::interpret::ResultPtr indirect(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter);

  static FMA::interpret::ResultPtr number_dp(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter);
  static FMA::interpret::ResultPtr number_addr(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter);
  static FMA::interpret::ResultPtr number_indirect(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter);

  static FMA::interpret::ResultPtr function_jsr(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter);

};

}
}

#endif
