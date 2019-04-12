#ifndef __FMA_INSTRUCT_DYNAMICBINARYDATA_H__
#define __FMA_INSTRUCT_DYNAMICBINARYDATA_H__

#include "../assem/Instruction.hpp"
#include "../symbol/Reference.hpp"

namespace FMA {
namespace instruct {

class DynamicBinaryData : public assem::Instruction0 {
public:
  DynamicBinaryData(const symbol::ReferencePtr &data, uint32_t size) : data(data), size(size) {}

  virtual std::string getName();
  virtual std::string asString();
  virtual std::string getIdentifier() { return "$SYM"; }
  
  virtual const symbol::ReferencePtr &getData() const { return data; }
  virtual uint32_t getSize() const { return size; }

protected:
  symbol::ReferencePtr data;
  uint32_t size;
};

}
}

#endif