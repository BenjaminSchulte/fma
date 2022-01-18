#pragma once

#include <stdint.h>

namespace FmaCpp {

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

void gameAllocate();
void gameFree();

uint8_t romRead(uint32_t address);
int32_t gameResume(int32_t address);

}