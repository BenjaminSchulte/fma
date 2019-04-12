#ifndef __FMA_MEMORY_PLUGIN_H__
#define __FMA_MEMORY_PLUGIN_H__

#include <fma/plugin/Plugin.hpp>
#include <fma/plugin/MemoryPluginAdapter.hpp>

namespace FMA {
namespace interpret {
class BaseContext;
struct GroupedParameterList;
typedef std::shared_ptr<BaseContext> ContextPtr;
typedef std::shared_ptr<class Result> ResultPtr;
}
namespace memory {

class MemoryPlugin : public plugin::MemoryManagerPlugin {  
public:
  MemoryPlugin(Project *project);

  const char *getName() const {
    return "FMA memory management plugin";
  }

  const char *getDescription() const {
    return "Automatic memory management for FMA";
  }

  static interpret::ResultPtr scope(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr declare(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr locate_at(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr class_pointer(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  virtual plugin::MemoryPluginAdapter *createAdapter();
  virtual void releaseAdapter(plugin::MemoryPluginAdapter *adapter);

  bool initialize();
  void release();
};

}
}

#endif
