#pragma once

#include "../output/DynamicBuffer.hpp"

namespace FMA {
class Log;  
namespace linker {
class LinkerObject;
class LinkerBlock;

class LinkerObjectDeserializer {
public:
  //! Constructor
  LinkerObjectDeserializer(Log *log, LinkerObject *object)
    : mLog(log)
    , mObject(object) {}

  //! Deserializes the object
  bool deserialize(output::DynamicBuffer &buffer) const;

  //! Deserializes the object by file
  bool deserialize(const char *fileName) const;

private:
  //! Deserializes the blocks
  bool deserializeBlocks(output::DynamicBuffer &buffer) const;

  //! Deserialize a blocks
  bool deserializeBlock(output::DynamicBuffer &buffer) const;

  //! The log
  Log *mLog;

  //! The linker object to serialize
  LinkerObject *mObject;
};

}
}