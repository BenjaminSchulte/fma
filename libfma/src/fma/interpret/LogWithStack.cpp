#include <boost/range/adaptor/reversed.hpp>
#include <fma/interpret/Interpreter.hpp>
#include <fma/interpret/LogWithStack.hpp>
#include <fma/Project.hpp>
#include <ostream>

using namespace FMA;
using namespace FMA::interpret;

// ----------------------------------------------------------------------------
StackLogMessage::StackLogMessage(Interpreter *interpreter, LogLevel level)
  : level(level)
  , interpreter(interpreter)
{
}

// ----------------------------------------------------------------------------
StackLogMessage::~StackLogMessage() {
  auto &log = interpreter->getProject()->log();

  log.write(level, os.str());
  
  for (auto &line : boost::adaptors::reverse(interpreter->stack().all())) {
    if (!line.ref.isValid()) {
      continue;
    }

    std::ostringstream os;
    os << "  in: " << line.ref.asString();
    log.write(level, os.str());
  }
  log.write(level, "  - end of stack -");
}

