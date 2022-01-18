#ifndef __FMA_INSTRUCT_RAWCODE_H__
#define __FMA_INSTRUCT_RAWCODE_H__

#include "../assem/Instruction.hpp"

namespace FMA {
namespace instruct {

class RawCode : public assem::Instruction0 {
public:
  RawCode(const std::string &data) : data(data) {}

  virtual std::string getName() { return "RAWCODE"; }
  virtual std::string asString() { return data; }
  virtual std::string getIdentifier() { return "$RAW"; }

  virtual const std::string &getData() const { return data; }

protected:
  std::string data;
};

}
}

#endif