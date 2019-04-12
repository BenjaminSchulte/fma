#ifndef __FMACSV_CSV_PLUGIN_H__
#define __FMACSV_CSV_PLUGIN_H__

#include <fma/plugin/Plugin.hpp>

namespace FMA {
namespace csv {

class CsvPlugin : public FMA::plugin::LanguagePlugin {  
public:
  CsvPlugin(FMA::Project *project);

  const char *getName() const {
    return "CSV processing features";
  }

  const char *getDescription() const {
    return "Adds csv processing features";
  }

  bool initialize();
  void release();

protected:
};

}
}

#endif
