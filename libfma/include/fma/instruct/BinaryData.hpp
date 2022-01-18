#ifndef __FMA_INSTRUCT_BINARYDATA_H__
#define __FMA_INSTRUCT_BINARYDATA_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class BinaryData : public assem::Instruction0 {
public:
  BinaryData(const void *data, uint32_t size);
  ~BinaryData();

  virtual std::string getName();
  virtual std::string asString();
  virtual std::string getIdentifier() { return "$DATA"; }

  virtual bool containsBinaryData() const { return true; }
  virtual void appendBinaryData(const void*, uint32_t);

  virtual uint32_t getSize() const { return size; }
  virtual const void *getData() const { return data; }

protected:
  void *data;
  uint32_t size;
  uint32_t memoryOffset;
  uint32_t memorySize;
};

}
}

#endif