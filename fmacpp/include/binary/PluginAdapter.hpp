#ifndef __FMACPP_BINARY_PLUGINADAPTER_H__
#define __FMACPP_BINARY_PLUGINADAPTER_H__

#include <map>
#include <fma/plugin/Plugin.hpp>
#include <fma/plugin/BinaryGeneratorPluginAdapter.hpp>

namespace FMACPP {
namespace binary {

class CodeSection;

struct ByteCodeGenerator;
typedef std::function<bool(const ByteCodeGenerator &generator, FMA::assem::BinaryCodeGeneratorScope *scope, FMA::assem::Instruction *instruct)> ByteCodeGeneratorFunc;

struct ByteCodeGenerator {
  ByteCodeGenerator() {}
  ByteCodeGenerator(const std::string &opcode, const ByteCodeGeneratorFunc &func)
    : opcode(opcode), func(func) {}

  inline bool execute(FMA::assem::BinaryCodeGeneratorScope *scope, FMA::assem::Instruction *instruct) {
    return func(*this, scope, instruct);
  }

  std::string opcode;
  ByteCodeGeneratorFunc func;
};

typedef std::map<std::string, ByteCodeGenerator> ByteCodeGeneratorMap;

class PluginAdapter : public FMA::plugin::BinaryGeneratorPluginAdapter {  
public:
  PluginAdapter(FMA::Project *project);
  ~PluginAdapter();

  bool supports(const std::string &name) const;
  bool generate(FMA::assem::BinaryCodeGeneratorScope *scope, FMA::assem::Instruction *instruct, const std::string &name);
  bool finish() override;

protected:
  ByteCodeGeneratorMap map;

  bool writeCode(CodeSection *section, const std::string &opcode, FMA::assem::Instruction*, bool);

  void label(const std::string &opcode, const std::string &name);
  void data(const std::string &opcode, const std::string &name);
  void symbol(const std::string &opcode, const std::string &name);
  void raw(const std::string &opcode, const std::string &name);

  void code(const std::string &opcode, const std::string &name);
  void codeReturn(const std::string &opcode, const std::string &name);
  void codeBreak(const std::string &opcode, const std::string &name);
  void ignore(const std::string &opcode, const std::string &name);

  void absLong(const std::string &opcode, const std::string &name);

  std::vector<CodeSection*> mCodeSections;
  CodeSection *mCurrentCodeSection = nullptr;

  int mNextLabelId = 0;
};

}
}

#endif
