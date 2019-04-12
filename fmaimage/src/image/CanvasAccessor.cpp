#include <image/CanvasAccessor.hpp>
#include <iostream>
#include <sstream>

using namespace FMA::image;

// ----------------------------------------------------------------------------
CanvasPtr BaseCanvasAccessor::flipX() const {
  CanvasPtr canvas = clone();
  canvas->flipX();
  return canvas;
}

// ----------------------------------------------------------------------------
CanvasPtr BaseCanvasAccessor::flipY() const {
  CanvasPtr canvas = clone();
  canvas->flipY();
  return canvas;
}

// ----------------------------------------------------------------------------
void BaseCanvasAccessor::dump() const {
  uint32_t w = width();
  uint32_t h = height();

  std::cout << "Canvas(" << w << "x" << h << ")" << std::endl;

  for (uint32_t y=0; y<h; y++) {
    std::ostringstream os;

    for (uint32_t x=0; x<w; x++) {
      os << get(x, y).asPrintableChar();
    }

    std::cout << os.str() << std::endl;
  }
}

// ----------------------------------------------------------------------------
CanvasPtr CanvasAccessor::clone() const {
  return canvas->clone();
}

// ----------------------------------------------------------------------------
CanvasPtr CanvasViewAccessor::clone() const {
  return canvas->clone();
}

// ----------------------------------------------------------------------------
CanvasPtr PaletteAccessor::clone() const {
  return Canvas::create(1, 1);
}