#ifndef __FMAIMAGE_IMAGE_CANVAS_H__
#define __FMAIMAGE_IMAGE_CANVAS_H__

#include "Palette.hpp"

namespace FMA {
namespace image {

typedef std::shared_ptr<class Canvas> CanvasPtr;

class Canvas {
public:
  Canvas();
  virtual ~Canvas();

  static CanvasPtr create(uint32_t width, uint32_t height, uint8_t bpp=4);
  static CanvasPtr createRaw(uint32_t width, uint32_t height, uint8_t bpp, const PalettePtr &palette, void *data);
  CanvasPtr clone() const;
  CanvasPtr slice(uint32_t left, uint32_t top, uint32_t width, uint32_t height) const;

  std::string hash() const;
  std::string hash(uint32_t left, uint32_t top, uint32_t width, uint32_t height) const;

  void draw(const Canvas *canvas, int32_t dx, int32_t dy, int32_t sx, int32_t sy, uint32_t w, uint32_t h);
  
  void unload();
  bool loadFromPng(const char *fileName);

  inline uint32_t getWidth() const { return width; }
  inline uint32_t getHeight() const { return height; }
  inline uint8_t getBytesPerPixel() const { return bytesPerPixel; }
  inline uint32_t getBytesPerRow() const { return bytesPerRow; }

  inline unsigned char *getData() const { return bytes; }
  inline unsigned char *getData(uint32_t col, uint32_t row) const { return bytes + row * bytesPerRow + col * bytesPerPixel; }
  inline unsigned char *getData(uint32_t row) const { return bytes + row * bytesPerRow; }

  void set(uint32_t col, uint32_t row, const Color &color);
  Color get(uint32_t col, uint32_t row);

  void clear();
  void fill(uint8_t colorIndex);
  void flipX();
  void flipY();

  inline bool hasPalette() const { return !!palette; }
  inline uint16_t getNumColors() const { return palette ? palette->getNumColors() : 0; }
  inline const PalettePtr &getPalette() { return palette; }
  inline const Color &getColor(uint8_t index) { return palette->getColor(index); }

  void convertToRgba();
  void applyPalette(const PalettePtr &palette);

protected:
  unsigned char *bytes;
  uint8_t bytesPerPixel;
  uint32_t bytesPerRow;
  uint32_t width;
  uint32_t height;
  PalettePtr palette;
};

}
}

#endif
