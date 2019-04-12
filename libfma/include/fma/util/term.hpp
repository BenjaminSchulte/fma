#ifndef __FMA_UTIL_TERM_H__
#define __FMA_UTIL_TERM_H__

#include <string>

namespace FMA {
namespace util {

enum Color {
  BLACK=0x0,
  RED=0x01,
  GREEN=0x02,
  YELLOW=0x03,
  BLUE=0x04,
  PINK=0x05,
  CYAN=0x06,
  WHITE=0x07
};

std::string setcolor(Color color, bool bright=false);
std::string nocolor();

namespace color {
  inline std::string black() { return setcolor(BLACK); }
  inline std::string red() { return setcolor(RED); }
  inline std::string green() { return setcolor(GREEN); }
  inline std::string yellow() { return setcolor(YELLOW); }
  inline std::string blue() { return setcolor(BLUE); }
  inline std::string pink() { return setcolor(PINK); }
  inline std::string cyan() { return setcolor(CYAN); }
  inline std::string white() { return setcolor(WHITE); }
  inline std::string blackBright() { return setcolor(BLACK, true); }
  inline std::string redBright() { return setcolor(RED, true); }
  inline std::string greenBright() { return setcolor(GREEN, true); }
  inline std::string yellowBright() { return setcolor(YELLOW, true); }
  inline std::string blueBright() { return setcolor(BLUE, true); }
  inline std::string pinkBright() { return setcolor(PINK, true); }
  inline std::string cyanBright() { return setcolor(CYAN, true); }
  inline std::string whiteBright() { return setcolor(WHITE, true); }
  inline std::string off() { return nocolor(); }
}

}
}

#endif
