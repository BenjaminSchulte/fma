#include <fma/Log.hpp>
#include <fma/util/term.hpp>
#include <iostream>

using namespace FMA;
using namespace FMA::util;

// ----------------------------------------------------------------------------
OriginalLogMessage::OriginalLogMessage(Log *log, LogLevel level)
  : level(level)
  , log(log)
{
}

// ----------------------------------------------------------------------------
OriginalLogMessage::~OriginalLogMessage() {
  log->write(level, os.str());
}


// ----------------------------------------------------------------------------
Log::Log()
  : numErrors(0)
{
}

// ----------------------------------------------------------------------------
void Log::write(LogLevel level, const std::string &message) {
  std::string levelName;
  std::string colorName("");
  std::string resetColorName("\x1b[m");

  switch (level) {
    case FMA_LOG_TRACE: levelName="TRACE"; colorName=color::blackBright(); break;
    case FMA_LOG_DEBUG: levelName="DEBUG"; colorName=color::blackBright(); break;
    default:
    case FMA_LOG_INFO: levelName="INFO "; break;
    case FMA_LOG_NOTICE: levelName="NOTIC"; colorName=color::yellowBright(); break;
    case FMA_LOG_WARN: levelName="WARN "; colorName=color::yellowBright(); break;
    case FMA_LOG_ERROR: levelName="ERROR"; numErrors++; colorName=color::redBright(); break;
    case FMA_LOG_FATAL: levelName="FATAL"; numErrors++; colorName=color::redBright(); break;
  }

  if (level >= FMA_LOG_WARN) {
    std::cout << colorName << "[" << levelName << "] " << message << color::off() << std::endl;
  } else {
    std::cout << colorName << "[" << levelName << "] " << message << color::off() << std::endl;
  }
}

// ----------------------------------------------------------------------------

