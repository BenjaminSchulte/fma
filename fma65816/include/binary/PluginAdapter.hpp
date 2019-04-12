#ifndef __FMA65816_BINARY_PLUGINADAPTER_H__
#define __FMA65816_BINARY_PLUGINADAPTER_H__

#include <map>
#include <fma/plugin/Plugin.hpp>
#include <fma/plugin/BinaryGeneratorPluginAdapter.hpp>

namespace FMA65816 {
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

class PluginAdapter : public FMA::plugin::BinaryGeneratorPluginAdapter {  
public:
  PluginAdapter(FMA::Project *project);

  bool supports(const std::string &name) const;
  bool generate(FMA::assem::BinaryCodeGeneratorScope *scope, FMA::assem::Instruction *instruct, const std::string &name);

protected:
  ByteCodeGeneratorMap map;

  void label(uint8_t opcode, const std::string &name);

  void stzAbs(uint8_t opcode, const std::string &name);
  void stzDp(uint8_t opcode, const std::string &name);
  void absolute(uint8_t opcode, const std::string &name);
  void absLong(uint8_t opcode, const std::string &name);
  void brk(uint8_t opcode, const std::string &name);
  void directPage(uint8_t opcode, const std::string &name);
  void sep(uint8_t opcode, const std::string &name);
  void rep(uint8_t opcode, const std::string &name);
  void immediate(uint8_t opcode, const std::string &name);
  void implicit(uint8_t opcode, const std::string &name);
  void pcRelative(uint8_t opcode, const std::string &name);
  void srRelative(uint8_t opcode, const std::string &name);
};

}
}

#endif
