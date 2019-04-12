#ifndef __FMAIMAGE_IMAGE_PLUGIN_H__
#define __FMAIMAGE_IMAGE_PLUGIN_H__

#include <fma/plugin/Plugin.hpp>

namespace FMA {
namespace image {

class ImagePlugin : public FMA::plugin::LanguagePlugin {  
public:
  ImagePlugin(FMA::Project *project);

  const char *getName() const {
    return "Image processing features";
  }

  const char *getDescription() const {
    return "Adds image processing features";
  }

  bool initialize();
  void release();

protected:
};

}
}

#endif
