#pragma once

#include <stdint.h>
#include <string>
#include <functional>

namespace FmaCpp {

// Logging levels
enum LogLevel {
  LOG_TODO,
  LOG_TRACE,
  LOG_DEBUG,
  LOG_INFO,
  LOG_NOTE,
  LOG_WARN,
  LOG_ERROR,
  LOG_FATAL
};

// The configuration registers
enum GraphicRegister {
  INIDISP = 0x2100,
  OBSEL   = 0x2101,
  OAMADDL = 0x2102,
  OAMADDH = 0x2103,
  OAMDATA = 0x2104,
  BGMODE  = 0x2105,
  MOSAIC  = 0x2106,
  BG1SC   = 0x2107,
  BG2SC   = 0x2108,
  BG3SC   = 0x2109,
  BG4SC   = 0x210A,
  BG12NBA = 0x210B,
  BG34NBA = 0x210C,
  BG1HOFS = 0x210D,
  BG1VOFS = 0x210E,
  BG2HOFS = 0x210F,
  BG2VOFS = 0x2110,
  BG3HOFS = 0x2111,
  BG3VOFS = 0x2112,
  BG4HOFS = 0x2113,
  BG4VOFS = 0x2114,
  VMAIN   = 0x2115,
  VMADDL  = 0x2116,
  VMADDH  = 0x2117,
  VMDATAL = 0x2118,
  VMDATAH = 0x2119,
  M7SEL   = 0x211A,
  M7A     = 0x211B,
  M7B     = 0x211C,
  M7C     = 0x211D,
  M7D     = 0x211E,
  M7X     = 0x211F,
  M7Y     = 0x2120,
  CGADD   = 0x2121,
  CGDATA  = 0x2122,
  W12SEL  = 0x2123,
  W34SEL  = 0x2124,
  WOBJSEL = 0x2125,
  WH0     = 0x2126,
  WH1     = 0x2127,
  WH2     = 0x2128,
  WH3     = 0x2129,
  WBGLOG  = 0x212A,
  WOBJLOG = 0x212B,
  TM      = 0x212C,
  TS      = 0x212D,
  TMW     = 0x212E,
  TSW     = 0x212F,
  CGWSEL  = 0x2130,
  CGADSUB = 0x2131,
  COLDATA = 0x2132,
  SETINI  = 0x2133,
};

// The system register
enum SystemRegister {
  NMITIMEN = 0x4200,
  WRIO     = 0x4201,
  WRMPYA   = 0x4202,
  WRMPYB   = 0x4203,
  WRDIVL   = 0x4204,
  WRDIVH   = 0x4205,
  WRDIVB   = 0x4206,
  HTIMEL   = 0x4207,
  HTIMEH   = 0x4208,
  VTIMEL   = 0x4209,
  VTIMEH   = 0x420A,
  MDMAEN   = 0x420B,
  HDMAEN   = 0x420C,
  MEMSEL   = 0x420D,
};

// The engine adapter
struct EngineAdapter {
  std::function<void(GraphicRegister reg, int value)> setGraphicRegister;
  std::function<void(SystemRegister reg, int value)> setSystemRegister;
  std::function<void(const std::string &, LogLevel)> log;
};

// Initializers
void gameAllocate(const EngineAdapter &adapter);
void gameFree();

// Memory access
uint8_t romRead(uint32_t address);

// Game control
int32_t gameResume(int32_t address);

}