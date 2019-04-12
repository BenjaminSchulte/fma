#include <image/Plugin.hpp>
#include <fma/Project.hpp>
#include <fma/types/Module.hpp>
#include <fma/types/Class.hpp>
#include <fma/types/RootModule.hpp>

#include <image/CanvasClass.hpp>
#include <image/CanvasArrayClass.hpp>
#include <image/BaseCanvasClass.hpp>
#include <image/CanvasViewClass.hpp>
#include <image/ImageClass.hpp>
#include <image/PixelClass.hpp>
#include <image/PaletteClass.hpp>
#include <image/ImageCollectionClass.hpp>
#include <image/ImageCollectionItemClass.hpp>
#include <image/PaletteArrayClass.hpp>

#include <fma/serialize/SerializerRegistry.hpp>

using namespace FMA;
using namespace FMA::image;
using namespace FMA::types;

// ----------------------------------------------------------------------------
ImagePlugin::ImagePlugin(Project *project)
  : plugin::LanguagePlugin(project)
{
}

// ----------------------------------------------------------------------------
bool ImagePlugin::initialize() {
  ModulePtr root(new Module("fmaImage"));

  const RootModulePtr &_root = project->root();
  _root->setMember("fmaImage", root);

  const ClassPtr &klass = _root->getMember("Class")->asClass();
  const ClassPtr &Array = _root->getMember("Array")->asClass();
  if (!klass || !Array) {
    return false;
  }

  project->serializer()->registerClass("fmaImage::Pixel", PixelClass::create(root, klass));
  project->serializer()->registerClass("fmaImage::Palette", PaletteClass::create(root, klass));
  project->serializer()->registerClass("fmaImage::CanvasArray", CanvasArrayClass::create(root, Array));
  project->serializer()->registerClass("fmaImage::PaletteArray", PaletteArrayClass::create(root, Array));
  project->serializer()->registerClass("fmaImage::ImageCollection", ImageCollectionClass::create(root, klass));
  project->serializer()->registerClass("fmaImage::ImageCollectionItem", ImageCollectionItemClass::create(root, klass));

  const ClassPtr &BaseCanvas = BaseCanvasClass::create(root, klass);
  project->serializer()->registerClass("fmaImage::CanvasView", CanvasViewClass::create(root, BaseCanvas));

  const ClassPtr &Canvas = CanvasClass::create(root, BaseCanvas);
  project->serializer()->registerClass("fmaImage::Image", ImageClass::create(root, Canvas));

  project->serializer()->registerClass("fmaImage::Canvas", Canvas);
  

  return true;
}

// ----------------------------------------------------------------------------
void ImagePlugin::release() {
}
