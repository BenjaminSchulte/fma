#include <image/Color.hpp>
#include <fma/util/term.hpp>
#include <sstream>

using namespace FMA::image;

// ----------------------------------------------------------------------------
ColorHsl Color::hsl() const {
  float r = (float)this->r / 255.0f;
  float g = (float)this->g / 255.0f;
  float b = (float)this->b / 255.0f;

  float min = std::min(std::min(r, g), b);
  float max = std::max(std::max(r, g), b);
  float delta = max - min;

  int h;
  float s;
  float l = (max + min) / 2;

  if (delta == 0) {
    h = 0;
    s = 0.0f;
  } else {
    s = (l <= 0.5) ? (delta / (max + min)) : (delta / (2 - max - min));
    
    float hue;
    if (r == max) {
      hue = ((g - b) / 6) / delta;
    } else if (g == max) {
      hue = (1.0f / 3) + ((b - r) / 6) / delta;
    } else {
      hue = (2.0f / 3) + ((r - g) / 6) / delta;
    }

		if (hue < 0){
			hue += 1;
    } else if (hue > 1) {
			hue -= 1;
    }

    h = hue * 360;
  }

  return ColorHsl(h, s, l, a);
}

// ----------------------------------------------------------------------------
std::string Color::asPrintableChar() const {
  auto color = hsl();
  std::ostringstream os;

  if (isTransparent()) {
    os << util::color::blackBright() << ".";
  } else if (color.l < 0.2) {
    os << util::color::blackBright() << "x";
  } else if (color.l > 0.8) {
    os << util::color::whiteBright() << "x";
  } else if (color.s < 0.2) {
    os << util::color::blackBright() << "x";
  } else {
    if (color.l < 0.5) {
      if (color.h < 30) { os << util::color::red() << "x"; }
      else if (color.h < 90) { os << util::color::yellow() << "x"; }
      else if (color.h < 150) { os << util::color::green() << "x"; }
      else if (color.h < 210) { os << util::color::cyan() << "x"; }
      else if (color.h < 270) { os << util::color::blue() << "x"; }
      else if (color.h < 330) { os << util::color::pink() << "x"; }
      else { os << util::color::red() << "x"; }
    } else {
      if (color.h < 30) { os << util::color::redBright() << "#"; }
      else if (color.h < 90) { os << util::color::yellowBright() << "#"; }
      else if (color.h < 150) { os << util::color::greenBright() << "#"; }
      else if (color.h < 210) { os << util::color::cyanBright() << "#"; }
      else if (color.h < 270) { os << util::color::blueBright() << "#"; }
      else if (color.h < 330) { os << util::color::pinkBright() << "#"; }
      else { os << util::color::redBright() << "#"; }
    }
  }

  os << util::color::off();
  return os.str();
}

// ----------------------------------------------------------------------------
uint32_t Color::distanceTo(const Color &other) const {
  if (isTransparent() && other.isTransparent()) {
    return 0;
  }

  if (isTransparent() != other.isTransparent()) {
    return 0xFFFFFFFF;
  }

  uint16_t dr = abs(r - other.r);
  uint16_t dg = abs(g - other.g);
  uint16_t db = abs(b - other.b);

  return dr * dr + dg * dg + db * db;
}

// ----------------------------------------------------------------------------