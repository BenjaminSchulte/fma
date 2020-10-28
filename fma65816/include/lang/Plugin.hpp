#ifndef __FMA65816_LANG_PLUGIN_H__
#define __FMA65816_LANG_PLUGIN_H__

#include <fma/plugin/Plugin.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/ParameterList.hpp>

namespace FMA65816 {
namespace lang {

class CallWrapper {
public:
  CallWrapper(const FMA::types::TypePtr &dataBlockCallOperator)
    : dataBlockCallOperator(dataBlockCallOperator)
  {
  }

  FMA::interpret::ResultPtr call_datablock(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter);

  FMA::types::TypePtr dataBlockCallOperator;
};

class LanguagePlugin : public FMA::plugin::LanguagePlugin {  
public:
  LanguagePlugin(FMA::Project *project);

  const char *getName() const {
    return "65816 language support";
  }

  const char *getDescription() const {
    return "Adds instructions used for 65816 development";
  }

  bool initialize();
  void release();

protected:
  static FMA::interpret::ResultPtr get_register_size(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter);
  static FMA::interpret::ResultPtr set_register_size(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter);

  static void setFlagRegister(const FMA::interpret::ContextPtr &context, uint8_t value);
  static void resetFlagRegister(const FMA::interpret::ContextPtr &context, uint8_t value);
  static uint8_t getImmediateSize(const FMA::interpret::ContextPtr &context, const std::string &registerName);

  static void transferRegisterType(const FMA::interpret::ContextPtr &context, const std::string &from, const std::string &to);

  static FMA::interpret::ResultPtr _register(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter);
  static FMA::interpret::ResultPtr param(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter);
  static FMA::interpret::ResultPtr returns(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter);
  static FMA::interpret::ResultPtr dp(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter);
  static FMA::interpret::ResultPtr long_address(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter);
  static FMA::interpret::ResultPtr number_dp(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter);
  static FMA::interpret::ResultPtr number_long_address(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter);
  static FMA::interpret::ResultPtr indirect(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter);
  static FMA::interpret::ResultPtr number_indirect(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter);
  static FMA::interpret::ResultPtr function_jsr(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter);
  static FMA::interpret::ResultPtr auto_insert_rts(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter);
  static FMA::interpret::ResultPtr bank(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter);
  static FMA::interpret::ResultPtr bank_address(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter);
  static FMA::interpret::ResultPtr compiler_break(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter);

  static void testTypeHinting(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter, const char *registerName);
  static void testTypeHinting(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter);
  static void testTypeHinting(const FMA::interpret::ContextPtr &context);

  CallWrapper *callWrapper;
};

}
}

#endif
