#pragma once

#include "../output/DynamicBuffer.hpp"

namespace FMA {
namespace linker {
class LinkerObject;
class LinkerBlock;

class LinkerObjectSerializer {
public:
  //! Constructor
  LinkerObjectSerializer(LinkerObject *object) : mObject(object) {}

  //! Serializes the object
  bool serialize();

  //! Returns the buffer
  inline const output::DynamicBuffer &buffer() const { return mBuffer; }

private:
  //! Serializes all blocks
  bool serializeBlocks();

  //! Serializes a block
  bool serializeBlock(const LinkerBlock *);

  //! The linker object to serialize
  LinkerObject *mObject;

  //! The output write buffer
  output::DynamicBuffer mBuffer;
};

}
}