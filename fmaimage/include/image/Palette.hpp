#ifndef __FMAIMAGE_IMAGE_PALETTE_H__
#define __FMAIMAGE_IMAGE_PALETTE_H__

#include "Color.hpp"

namespace FMA {
namespace image {

typedef std::shared_ptr<class Palette> PalettePtr;

class Palette {
public:
  Palette() : colors() {}
  Palette(const ColorList &colors) : colors(colors) {}
  ~Palette();

  inline uint16_t getNumColors() const { return colors.size(); }
  uint16_t findBest(const Color &color) const;
  inline const Color &getColor(uint16_t index) { return colors[index]; }
  bool contains(const Color &color) const;
  bool contains(const PalettePtr &other) const;
  bool equals(const PalettePtr &other) const;
  PalettePtr slice(uint16_t offset, uint16_t size) const;
  PalettePtr clone() const;
  void setColor(uint16_t index, const Color &);
  void add(const Color &);
  void clear();
  
protected:
  ColorList colors;
};

}
}

#endif
