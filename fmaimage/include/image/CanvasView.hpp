#ifndef __FMAIMAGE_IMAGE_CANVASVIEW_H__
#define __FMAIMAGE_IMAGE_CANVASVIEW_H__

#include "Canvas.hpp"

namespace FMA {
namespace image {

typedef std::shared_ptr<class CanvasView> CanvasViewPtr;

class CanvasView {
public:
  CanvasView(const CanvasPtr &parent, uint32_t left, uint32_t top, uint32_t width, uint32_t height);
  CanvasView(const CanvasViewPtr &parent, uint32_t left, uint32_t top, uint32_t width, uint32_t height);
  virtual ~CanvasView();

  CanvasPtr clone() const;
  std::string hash() const;
  
  inline void set(uint32_t col, uint32_t row, const Color &color) {
    canvas->set(col + left, row + top, color);
  }

  inline Color get(uint32_t col, uint32_t row) {
    return canvas->get(col + left, row + top);
  }

  inline const CanvasPtr &getCanvas() const { return canvas; }
  inline uint32_t getTop() const { return top; }
  inline uint32_t getLeft() const { return left; }
  inline uint32_t getWidth() const { return width; }
  inline uint32_t getHeight() const { return height; }

protected:
  CanvasPtr canvas;
  uint32_t left;
  uint32_t top;
  uint32_t width;
  uint32_t height;
};

}
}

#endif
