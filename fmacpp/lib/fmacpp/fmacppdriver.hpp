#pragma once

#include <string>
#include <iostream>
#include "fmacpplib.hpp"

using namespace FmaCpp;

namespace {
EngineAdapter lib;

void printLog(const std::string &message, LogLevel level);

// -----------------------------------------------------------------------------
//   Registers
// -----------------------------------------------------------------------------
uint16_t R_A;
uint16_t R_X;
uint16_t R_Y;
uint16_t R_S;
uint16_t R_D;
uint8_t R_PC_BANK;
uint8_t R_DB;

bool P_C;
bool P_Z;
bool P_I;
bool P_D;
bool P_X;
bool P_M;
bool P_V;
bool P_N;

uint8_t *ram;

uint8_t P() {
  return (
    (P_C ? 0x01 : 0)
    | (P_Z ? 0x02 : 0)
    | (P_I ? 0x04 : 0)
    | (P_D ? 0x08 : 0)
    | (P_X ? 0x10 : 0)
    | (P_M ? 0x20 : 0)
    | (P_V ? 0x40 : 0)
    | (P_N ? 0x80 : 0)
  );
}
#define R_P P()

// -----------------------------------------------------------------------------
//   Data access
// -----------------------------------------------------------------------------
uint8_t dataRead8(uint32_t address) {
  if (address >= 0x0000 && address < 0x2000) {
    return ram[address];
  } else if (address >= 0x7E0000 && address <= 0x7FFFFF) {
    return ram[address & 0x1FFFF];
  }
  return romRead(address);
}

inline uint16_t dataRead16(uint32_t address) {
  return dataRead8(address) | dataRead8(address + 1) << 8;
}

void dataWrite8(uint32_t address, uint8_t value) {
  if (address >= 0x0000 && address < 0x2000) {
    ram[address] = value;
  } else if (address >= 0x2100 && address < 0x2140) {
    if (lib.setGraphicRegister) { lib.setGraphicRegister((GraphicRegister)address, value); }
  } else if (address >= 0x4200 && address < 0x4220) {
    if (lib.setSystemRegister) { lib.setSystemRegister((SystemRegister)address, value); }
  } else if (address >= 0x7E0000 && address <= 0x7FFFFF) {
    ram[address & 0x1FFFF] = value;
  } else {
    printLog(std::string("Invalid WRITE access at address ") + std::to_string(address), LOG_ERROR);
  }
}

void dataWrite16(uint32_t address, uint8_t value) {
  dataWrite8(address, value);
  dataWrite8(address + 1, value >> 8);
}

// -----------------------------------------------------------------------------
//   Stack
// -----------------------------------------------------------------------------
inline void PUSH8(uint8_t value) {
  dataWrite8(R_S--, value);
}

inline void PUSH16(uint16_t value) {
  PUSH8(value >> 8);
  PUSH8(value);
}

uint8_t POP8() {
  return dataRead8(++R_S);
}

uint16_t POP16() {
  return POP8() | (POP8() << 8);
}


// -----------------------------------------------------------------------------
//   Registers
// -----------------------------------------------------------------------------
inline void setPC(int32_t address) {
  R_PC_BANK = (uint32_t)address >> 16;
}

void SEP(uint8_t bits) {
  if (bits & 0x01) { P_C = true; }
  if (bits & 0x02) { P_Z = true; }
  if (bits & 0x04) { P_I = true; }
  if (bits & 0x08) { P_D = true; }
  if (bits & 0x10) { P_X = true; }
  if (bits & 0x20) { P_M = true; }
  if (bits & 0x40) { P_V = true; }
  if (bits & 0x80) { P_N = true; }
}

void REP(uint8_t bits) {
  if (bits & 0x01) { P_C = false; }
  if (bits & 0x02) { P_Z = false; }
  if (bits & 0x04) { P_I = false; }
  if (bits & 0x08) { P_D = false; }
  if (bits & 0x10) { P_X = false; }
  if (bits & 0x20) { P_M = false; }
  if (bits & 0x40) { P_V = false; }
  if (bits & 0x80) { P_N = false; }
}



// -----------------------------------------------------------------------------
//   Routines
// -----------------------------------------------------------------------------
int32_t RTI() {
  return -1;
}

int32_t RTS() {
  return POP16() | R_PC_BANK << 16;
}

#define TEST8(value) \
  P_Z = value & 0xFF == 0; \
  P_N = value & 0x80 != 0;

#define TEST16(value) \
  P_Z = value & 0xFFFF == 0; \
  P_N = value & 0x8000 != 0;

void _LDA8(uint8_t value) {
  R_A = (R_A & 0xFF00) | value;
  TEST8(value);
}
#define LDA8(v) _LDA8((v) & 0xFF)

void _LDA16(uint16_t value) {
  R_A = value;
  TEST16(value);
}
#define LDA16(v) _LDA16((v) & 0xFFFF)

void _LDX8(uint8_t value) {
  R_X = (R_X & 0xFF00) | value;
  TEST8(value);
}
#define LDX8(v) _LDX8((v) & 0xFF)

void _LDX16(uint16_t value) {
  R_X = value;
  TEST16(value);
}
#define LDX16(v) _LDX16((v) & 0xFF)

void _LDY8(uint8_t value) {
  R_Y = (R_Y & 0xFF00) | value;
  TEST8(value);
}
#define LDY8(v) _LDY8((v) & 0xFF)

void _LDY16(uint16_t value) {
  R_Y = value;
  TEST16(value);
}
#define LDY16(v) _LDY16((v) & 0xFF)

int32_t CALL(int32_t address, int32_t returnAddress) {
  PUSH16(returnAddress);
  return address;
}



// -----------------------------------------------------------------------------
//   Helper routines
// -----------------------------------------------------------------------------
inline int32_t currentBank(int32_t value) {
  return (value & 0xFFFF) | (R_DB << 16);
}


// -----------------------------------------------------------------------------
//   Engine callbacks
// -----------------------------------------------------------------------------
void printLog(const std::string &message, LogLevel level) {
  if (lib.log) { lib.log(message, level); }
}

}

// -----------------------------------------------------------------------------
//   Direct setup
// -----------------------------------------------------------------------------
void FmaCpp::gameAllocate(const EngineAdapter &adapter) {
  lib = adapter;
  ram = (uint8_t*)malloc(0x10000 * 2);
}

void FmaCpp::gameFree() {
  free(ram);
}