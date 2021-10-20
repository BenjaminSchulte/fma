#pragma once

#include <fma/plugin/Plugin.hpp>

namespace FMA {
namespace interpret {
typedef std::shared_ptr<class BaseContext> ContextPtr;
typedef std::shared_ptr<class Result> ResultPtr;
struct GroupedParameterList;
}

namespace superfx {

class SuperFxPlugin : public FMA::plugin::LanguagePlugin {  
public:
  SuperFxPlugin(FMA::Project *project);

  const char *getName() const {
    return "SuperFX processing features";
  }

  const char *getDescription() const {
    return "Adds SuperFX processing features";
  }

  bool initialize();
  void release();

protected:
  static FMA::interpret::ResultPtr superfx(const FMA::interpret::ContextPtr &context, const FMA::interpret::GroupedParameterList &parameter);
};

}
}
