#include <image/ImageCollection.hpp>
#include <iostream>

using namespace FMA::image;

// ----------------------------------------------------------------------------
ImageCollectionItem ImageCollection::add(const BaseCanvasAccessorPtr &canvas) {
  ImageCollectionItemMap::iterator it;

  std::string defaultHash = canvas->hash();
  it = items.find(defaultHash);
  if (it != items.end()) {
    return it->second;
  }

  uint32_t index = tiles.size();
  tiles.push_back(canvas->clone());

  addMirrorVariant(canvas, index, true, true);
  addMirrorVariant(canvas, index, true, false);
  addMirrorVariant(canvas, index, false, true);

  return addMirrorVariant(canvas, index, false, false);
}

// ----------------------------------------------------------------------------
ImageCollectionItem ImageCollection::addMirrorVariant(const BaseCanvasAccessorPtr &canvas, uint32_t index, bool flipX, bool flipY) {
  CanvasPtr current = canvas->clone();

  if (flipX) {
    current->flipX();
  }

  if (flipY) {
    current->flipY();
  }

  std::string hash = current->hash();
  ImageCollectionItem item(index, flipX, flipY);
  items[hash] = item;
  return item;
}

// ----------------------------------------------------------------------------
