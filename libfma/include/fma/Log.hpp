#ifndef __FMA_LOG_H__
#define __FMA_LOG_H__

#include <memory>
#include <sstream>

namespace FMA {

enum LogLevel {
  FMA_LOG_TRACE,
  FMA_LOG_DEBUG,
  FMA_LOG_INFO,
  FMA_LOG_NOTICE,
  FMA_LOG_WARN,
  FMA_LOG_ERROR,
  FMA_LOG_FATAL
};

class Log;

class BaseLogMessage {
public:
  BaseLogMessage() {}
  virtual ~BaseLogMessage() {}

  virtual std::ostringstream &stream() = 0;
};

class OriginalLogMessage : public BaseLogMessage {
public:
  OriginalLogMessage(Log *log, LogLevel level);
  ~OriginalLogMessage();

  std::ostringstream &stream() {
    return os;
  };

protected:
  std::ostringstream os;
  LogLevel level;
  Log *log;
};

struct LogMessage : public std::shared_ptr<BaseLogMessage> {
  LogMessage() : std::shared_ptr<BaseLogMessage>() {}
  LogMessage(const LogMessage &msg) : std::shared_ptr<BaseLogMessage>(msg) {}
  LogMessage(BaseLogMessage *msg) : std::shared_ptr<BaseLogMessage>(msg) {}

  template <typename T>
  LogMessage operator<<(T value) {
    get()->stream() << value;
    return *this;
  }
};

class LogAdapter {
public:
  LogAdapter() = default;
  virtual ~LogAdapter() = default;

  virtual void write(LogLevel level, const std::string &message) = 0;
};

class ConsoleLog : public LogAdapter {
public:
  void write(LogLevel level, const std::string &message) override;
};

class Log {
public:

  Log();

  inline LogMessage trace() {
    return LogMessage(new OriginalLogMessage(this, FMA_LOG_TRACE));
  }

  inline LogMessage debug() {
    return LogMessage(new OriginalLogMessage(this, FMA_LOG_DEBUG));
  }

  inline LogMessage info() {
    return LogMessage(new OriginalLogMessage(this, FMA_LOG_INFO));
  }

  inline LogMessage notice() {
    return LogMessage(new OriginalLogMessage(this, FMA_LOG_NOTICE));
  }

  inline LogMessage warn() {
    return LogMessage(new OriginalLogMessage(this, FMA_LOG_WARN));
  }

  inline LogMessage error() {
    return LogMessage(new OriginalLogMessage(this, FMA_LOG_ERROR));
  }

  inline LogMessage fatal() {
    return LogMessage(new OriginalLogMessage(this, FMA_LOG_FATAL));
  }

  bool hasErrors() const {
    return numErrors > 0;
  }

  void write(LogLevel level, const std::string &message);
  inline void setLogAdapter(LogAdapter *adapter) {
    logAdapter = adapter;
  }

private:
  uint32_t numErrors;
  LogLevel minimumLogLevel = FMA_LOG_DEBUG;
  LogAdapter *logAdapter;
};

}

#endif
