#ifndef __FMA_CORE_PLUGIN_H__
#define __FMA_CORE_PLUGIN_H__

#include "../plugin/Plugin.hpp"

namespace FMA {
namespace core {

class CorePlugin : public plugin::LanguagePlugin {  
public:
  CorePlugin(Project *project);

  const char *getName() const {
    return "FMA core plugin";
  }

  const char *getDescription() const {
    return "Basic objects and classes for FMA";
  }

  bool initialize();
  void release();
};

}
}

#endif
