#ifndef __FMAJSON_JSON_PLUGIN_H__
#define __FMAJSON_JSON_PLUGIN_H__

#include <fma/plugin/Plugin.hpp>

namespace FMA {
namespace json {

class JsonPlugin : public FMA::plugin::LanguagePlugin {  
public:
  JsonPlugin(FMA::Project *project);

  const char *getName() const {
    return "JSON processing features";
  }

  const char *getDescription() const {
    return "Adds json processing features";
  }

  bool initialize();
  void release();

protected:
};

}
}

#endif
