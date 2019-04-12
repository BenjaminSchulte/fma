#ifndef __FMA_CORE_COMPILER_H__
#define __FMA_CORE_COMPILER_H__

#include <fma/types/Class.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/Log.hpp>

namespace FMA {
namespace core {

class CompilerClass : public types::Class {
public:
  static types::ClassPtr create(const types::RootModulePtr &root, const types::ClassPtr &Class);

  // STATIC
  static interpret::ResultPtr PROJECT_DIR(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  static interpret::ResultPtr _assert(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr dump(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr print(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr command(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  static interpret::ResultPtr trace(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr debug(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr info(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr notice(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr warn(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr error(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr fatal(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  static interpret::ResultPtr _log(LogLevel level, const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static std::string _msg(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  // PROTOTYPE
};

}
}

#endif
