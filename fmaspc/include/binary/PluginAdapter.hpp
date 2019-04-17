#ifndef __FMASPC_BINARY_PLUGINADAPTER_H__
#define __FMASPC_BINARY_PLUGINADAPTER_H__

#include <map>
#include <fma/plugin/Plugin.hpp>
#include <fma/plugin/BinaryGeneratorPluginAdapter.hpp>

namespace FMASPC {
namespace binary {

struct ByteCodeGenerator;
typedef std::function<bool(const ByteCodeGenerator &generator, FMA::assem::BinaryCodeGeneratorScope *scope, FMA::assem::Instruction *instruct)> ByteCodeGeneratorFunc;

struct ByteCodeGenerator {
  ByteCodeGenerator() {}
  ByteCodeGenerator(uint8_t opcode, const ByteCodeGeneratorFunc &func)
    : opcode(opcode), func(func) {}

  inline bool execute(FMA::assem::BinaryCodeGeneratorScope *scope, FMA::assem::Instruction *instruct) {
    return func(*this, scope, instruct);
  }

  uint8_t opcode;
  ByteCodeGeneratorFunc func;
};

typedef std::map<std::string, ByteCodeGenerator> ByteCodeGeneratorMap;

class SpcPluginAdapter : public FMA::plugin::BinaryGeneratorPluginAdapter {  
public:
  SpcPluginAdapter(FMA::Project *project);

  bool supports(const std::string &name) const;
  bool generate(FMA::assem::BinaryCodeGeneratorScope *scope, FMA::assem::Instruction *instruct, const std::string &name);

protected:
  ByteCodeGeneratorMap map;

  void label(uint8_t opcode, const std::string &name);

  void implicit(uint8_t opcode, const std::string &name);
  void immediate(uint8_t opcode, const std::string &name);
  void absolute(uint8_t opcode, const std::string &name);
  void branchBit(uint8_t opcode, const std::string &name);
  void clrSetBit(uint8_t opcode, const std::string &name);
  void sep(uint8_t opcode, const std::string &name);
  void rep(uint8_t opcode, const std::string &name);
  void directPage(uint8_t opcode, const std::string &name);
  void dpImm(uint8_t opcode, const std::string &name);
  void dpImmBit(uint8_t opcode, const std::string &name);
  void dpPcRel(uint8_t opcode, const std::string &name);
  void dpPcRelNoA(uint8_t opcode, const std::string &name);
  void dpCopy(uint8_t opcode, const std::string &name);
  void pcRelative(uint8_t opcode, const std::string &name);
  void mov1CM(uint8_t opcode, const std::string &name);
  void mov1MC(uint8_t opcode, const std::string &name);

  static uint8_t bitToBitIndex(uint64_t value);
};

}
}

#endif
