#ifndef __FMAIMAGE_IMAGE_CANVASACCESSOR_H__
#define __FMAIMAGE_IMAGE_CANVASACCESSOR_H__

#include <string>
#include "Canvas.hpp"
#include "CanvasView.hpp"

namespace FMA {
namespace image {

typedef std::shared_ptr<class BaseCanvasAccessor> BaseCanvasAccessorPtr;

class BaseCanvasAccessor {
public:
  BaseCanvasAccessor() {}
  virtual ~BaseCanvasAccessor() {}

  virtual uint32_t width() const = 0;
  virtual uint32_t height() const = 0;

  virtual CanvasPtr clone() const = 0;  
  virtual std::string hash() const = 0;
  virtual Color get(uint32_t col, uint32_t row) const = 0;
  virtual void set(uint32_t col, uint32_t row, const Color &color) = 0;

  virtual CanvasViewPtr view(uint32_t left, uint32_t top, uint32_t width, uint32_t height) const = 0;

  virtual CanvasPtr flipX() const;
  virtual CanvasPtr flipY() const;

  void dump() const;
};

class CanvasAccessor : public BaseCanvasAccessor {
public:
  CanvasAccessor(const CanvasPtr &canvas) : canvas(canvas) {}
  virtual ~CanvasAccessor() {}

  virtual CanvasPtr clone() const;
  virtual uint32_t width() const { return canvas->getWidth(); }
  virtual uint32_t height() const { return canvas->getHeight(); }
  virtual std::string hash() const { return canvas->hash(); }
  
  virtual Color get(uint32_t col, uint32_t row) const { return canvas->get(col, row); }
  virtual void set(uint32_t col, uint32_t row, const Color &color) { canvas->set(col, row, color); }

  virtual CanvasViewPtr view(uint32_t left, uint32_t top, uint32_t width, uint32_t height) const {
    return CanvasViewPtr(new CanvasView(canvas, left, top, width, height));
  }

protected:
  CanvasPtr canvas;
};

class CanvasViewAccessor : public BaseCanvasAccessor {
public:
  CanvasViewAccessor(const CanvasViewPtr &canvas) : canvas(canvas) {}
  virtual ~CanvasViewAccessor() {}

  virtual CanvasPtr clone() const;
  virtual uint32_t width() const { return canvas->getWidth(); }
  virtual uint32_t height() const { return canvas->getHeight(); }
  virtual std::string hash() const { return canvas->hash(); }

  virtual Color get(uint32_t col, uint32_t row) const { return canvas->get(col, row); }
  virtual void set(uint32_t col, uint32_t row, const Color &color) { canvas->set(col, row, color); }

  virtual CanvasViewPtr view(uint32_t left, uint32_t top, uint32_t width, uint32_t height) const {
    return CanvasViewPtr(new CanvasView(canvas, left, top, width, height));
  }

protected:
  CanvasViewPtr canvas;
};

class PaletteAccessor : public BaseCanvasAccessor {
public:
  PaletteAccessor(const PalettePtr &palette) : palette(palette) {}
  virtual ~PaletteAccessor() {}

  virtual CanvasPtr clone() const;
  virtual uint32_t width() const { return palette->getNumColors(); }
  virtual uint32_t height() const { return 1; }
  virtual std::string hash() const { return ""; }
  
  virtual Color get(uint32_t col, uint32_t) const { return palette->getColor(col); }
  virtual void set(uint32_t col, uint32_t, const Color &color) { palette->setColor(col, color); }

  virtual CanvasViewPtr view(uint32_t, uint32_t, uint32_t, uint32_t) const {
    return CanvasViewPtr();
  }

protected:
  PalettePtr palette;
};

}
}

#endif
