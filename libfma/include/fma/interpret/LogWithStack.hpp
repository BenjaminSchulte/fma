#ifndef __FMA_INTERPRET_LOGWITHSTACK_H__
#define __FMA_INTERPRET_LOGWITHSTACK_H__

#include <fma/Log.hpp>

namespace FMA {
namespace interpret {

class Interpreter;

class StackLogMessage : public BaseLogMessage {
public:
  StackLogMessage(Interpreter *interpreter, LogLevel level);
  ~StackLogMessage();

  inline std::ostringstream &stream() {
    return os;
  };

protected:
  std::ostringstream os;
  LogLevel level;
  Interpreter *interpreter;
};

class LogWithStack {
public:

  LogWithStack(Interpreter *interpreter) : interpreter(interpreter) {}

  inline LogMessage trace() {
    return LogMessage(new StackLogMessage(interpreter, FMA::FMA_LOG_TRACE));
  }

  inline LogMessage debug() {
    return LogMessage(new StackLogMessage(interpreter, FMA::FMA_LOG_DEBUG));
  }

  inline LogMessage info() {
    return LogMessage(new StackLogMessage(interpreter, FMA::FMA_LOG_INFO));
  }

  inline LogMessage notice() {
    return LogMessage(new StackLogMessage(interpreter, FMA::FMA_LOG_NOTICE));
  }

  inline LogMessage warn() {
    return LogMessage(new StackLogMessage(interpreter, FMA::FMA_LOG_WARN));
  }

  inline LogMessage error() {
    return LogMessage(new StackLogMessage(interpreter, FMA::FMA_LOG_ERROR));
  }

  inline LogMessage fatal() {
    return LogMessage(new StackLogMessage(interpreter, FMA::FMA_LOG_FATAL));
  }

private:
  Interpreter *interpreter;
};

}
}

#endif
