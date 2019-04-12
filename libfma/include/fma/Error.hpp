#ifndef __FMA_ERROR_H__
#define __FMA_ERROR_H__

#include <string>
#include "Reference.hpp"

namespace FMA {

struct Error {
  /**
   * Different error types
   */
  enum ErrorType {
    INFO,
    NOTICE,
    WARNING,
    ERROR
  };

  Error(ErrorType type, const CodeReference &file, const std::string &message)
    : file(file), type(type), message(message)
  {
  }

  CodeReference file;
  ErrorType type;
  std::string message;
};

}

#endif
