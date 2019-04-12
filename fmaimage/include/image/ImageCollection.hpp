#ifndef __FMA_IMAGE_IMAGECOLLECTION_H__
#define __FMA_IMAGE_IMAGECOLLECTION_H__

#include <memory>
#include <vector>
#include <map>
#include "CanvasAccessor.hpp"
#include "Canvas.hpp"

namespace FMA {
namespace image {

typedef std::shared_ptr<class ImageCollection> ImageCollectionPtr;
typedef std::vector<CanvasPtr> CanvasList;

struct ImageCollectionItem {
  ImageCollectionItem() {}
  ImageCollectionItem(uint32_t index, bool flipX, bool flipY)
    : index(index), flipX(flipX), flipY(flipY) {}

  uint32_t index;
  bool flipX;
  bool flipY;
};

typedef std::map<std::string, ImageCollectionItem> ImageCollectionItemMap;

class ImageCollection {
public:
  ImageCollection() {}
  ~ImageCollection() {}

  ImageCollectionItem add(const BaseCanvasAccessorPtr &canvas);
  ImageCollectionItem addMirrorVariant(const BaseCanvasAccessorPtr &canvas, uint32_t index, bool flipX, bool flipY);

  inline uint32_t getNumUniqueImages() const { return tiles.size(); }
  inline uint32_t getNumVariants() const { return items.size(); }

protected:
  CanvasList tiles;
  ImageCollectionItemMap items;
};

}
}

#endif
