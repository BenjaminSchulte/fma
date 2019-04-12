#ifndef __FMAIMAGE_IMAGE_COLOR_H__
#define __FMAIMAGE_IMAGE_COLOR_H__

#include <stdlib.h>
#include <stdint.h>
#include <memory>
#include <vector>

namespace FMA {
namespace image {

struct ColorHsl {
  ColorHsl() : h(0), s(0), l(0), a(255) {}
  ColorHsl(int h, float s, float l) : h(h), s(s), l(l), a(255) {}
  ColorHsl(int h, float s, float l, uint8_t a) : h(h), s(s), l(l), a(a) {}

  int h;
  float s;
  float l;
  uint8_t a;
};

struct Color {
  Color() : r(0), g(0), b(0), a(255) {}
  Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b), a(255) {}
  Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;

  inline bool operator==(const Color &other) const {
    if (a == other.a && a == 0) {
      return true;
    }

    return r == other.r && g == other.g && b == other.b;
  }
  inline bool operator!=(const Color &other) const {
    return !(*this == other);
  }

  inline bool isTransparent() const { return a < 128; }
  
  static Color fromUint32(uint32_t value) {
    return Color(
      value & 0xFF,
      (value >> 8) & 0xFF,
      (value >> 16) & 0xFF,
      (value >> 24) & 0xFF
    );
  }

  inline uint32_t toUint32(bool fullAlpha=true) const {
    return r | (g << 8) | (b << 16) | (fullAlpha ? (a << 24) : (isTransparent() ? 0 : 255));
  }

  ColorHsl hsl() const;
  std::string asPrintableChar() const;
  uint32_t distanceTo(const Color &other) const;
};

typedef std::vector<Color> ColorList;

}
}

#endif