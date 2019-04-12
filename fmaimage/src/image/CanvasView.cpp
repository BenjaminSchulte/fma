#include <image/CanvasView.hpp>
#include <iostream>

using namespace FMA::image;

// ----------------------------------------------------------------------------
CanvasView::CanvasView(const CanvasPtr &parent, uint32_t _left, uint32_t _top, uint32_t _width, uint32_t _height)
  : canvas(parent)
{
  top = std::max<int32_t>(_top, 0);
  left = std::max<int32_t>(_left, 0);
  width = std::max<int32_t>(std::min(_width + left, parent->getWidth()) - left, 0);
  height = std::max<int32_t>(std::min(_height + top, parent->getHeight()) - top, 0);
}

// ----------------------------------------------------------------------------
CanvasView::CanvasView(const CanvasViewPtr &parent, uint32_t _left, uint32_t _top, uint32_t _width, uint32_t _height)
  : canvas(parent->getCanvas())
{
  _left += parent->getLeft();
  _top += parent->getTop();

  top = std::max<int32_t>(_top, 0);
  left = std::max<int32_t>(_left, 0);
  width = std::max<int32_t>(std::min(_width + left, parent->getWidth() + parent->getLeft()) - left, 0);
  height = std::max<int32_t>(std::min(_height + top, parent->getHeight() + parent->getTop()) - top, 0);
}

// ----------------------------------------------------------------------------
CanvasPtr CanvasView::clone() const {
  return canvas->slice(left, top, width, height);
}

// ----------------------------------------------------------------------------
std::string CanvasView::hash() const {
  return canvas->hash(left, top, width, height);
}

// ----------------------------------------------------------------------------
CanvasView::~CanvasView() {
}
