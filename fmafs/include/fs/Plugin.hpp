#ifndef __FMAFS_FS_PLUGIN_H__
#define __FMAFS_FS_PLUGIN_H__

#include <fma/plugin/Plugin.hpp>

namespace FMA {
namespace fs {

class FsPlugin : public FMA::plugin::LanguagePlugin {  
public:
  FsPlugin(FMA::Project *project);

  const char *getName() const {
    return "File system features";
  }

  const char *getDescription() const {
    return "Adds file system features";
  }

  bool initialize();
  void release();

protected:
};

}
}

#endif
