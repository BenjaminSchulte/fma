#include <image/Palette.hpp>
#include <iostream>

using namespace FMA::image;

// ----------------------------------------------------------------------------
Palette::~Palette() {
  clear();
}

// ----------------------------------------------------------------------------
void Palette::clear() {
  colors.clear();
}

// ----------------------------------------------------------------------------
void Palette::add(const Color &color) {
  colors.push_back(color);
}

// ----------------------------------------------------------------------------
uint16_t Palette::findBest(const Color &other) const {
  if (other.isTransparent()) {
    return 0;
  }
  
  uint32_t bestDistance = 0xFFFFFFFF;
  uint16_t bestColor = 0;

  uint16_t index = 0;
  for (const auto &color : colors) {
    if (index == 0) {
      index++;
      continue;
    }

    uint32_t distance = other.distanceTo(color);
    if (distance < bestDistance) {
      bestDistance = distance;
      bestColor = index;
    }

    index++;
  }

  return bestColor;
}

// ----------------------------------------------------------------------------
void Palette::setColor(uint16_t index, const Color &color) {
  while (colors.size() < index) {
    colors.push_back(Color());
  }

  colors.push_back(color);
}

// ----------------------------------------------------------------------------
PalettePtr Palette::clone() const {
  return PalettePtr(new Palette(colors));
}

// ----------------------------------------------------------------------------
PalettePtr Palette::slice(uint16_t offset, uint16_t size) const {
  ColorList newColors;

  uint16_t end = std::min<uint16_t>(colors.size(), offset + size);
  for (; offset<end; offset++) {
    newColors.push_back(colors.at(offset));
  }

  return PalettePtr(new Palette(newColors));
}

// ----------------------------------------------------------------------------
bool Palette::contains(const Color &other) const {
  for (const Color &color : colors) {
    if (color == other) {
      return true;
    }
  }

  return false;
}

// ----------------------------------------------------------------------------
bool Palette::contains(const PalettePtr &other) const {
  for (const Color &color : other->colors) {
    if (!contains(color)) {
      return false;
    }
  }

  return true;
}

// ----------------------------------------------------------------------------
bool Palette::equals(const PalettePtr &other) const {
  if (getNumColors() != other->getNumColors()) {
    return false;
  }

  uint32_t index = 0;
  for (const Color &color : other->colors) {
    if (color != colors.at(index++)) {
      return false;
    }
  }

  return true;
}

// ----------------------------------------------------------------------------
