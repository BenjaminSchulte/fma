#pragma once

#include "../output/DynamicBuffer.hpp"
#include "../plugin/MemoryPluginAdapter.hpp"

namespace FMA {
class Log;  
namespace linker {
class LinkerObject;
class LinkerBlock;

class LinkerObjectDeserializer {
public:
  //! Constructor
  LinkerObjectDeserializer(Log *log, plugin::MemoryPluginAdapter *plugin, LinkerObject *object)
    : mLog(log)
    , mMemoryPlugin(plugin)
    , mObject(object)
  {}

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

  //! The memory plugin
  plugin::MemoryPluginAdapter *mMemoryPlugin;

  //! The linker object to serialize
  LinkerObject *mObject;
};

}
}