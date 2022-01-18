#ifndef __FMA_PLUGIN_BINARYGENERATORPLUGINADAPTER_H__
#define __FMA_PLUGIN_BINARYGENERATORPLUGINADAPTER_H__

#include <memory>
#include "../types/InternalValue.hpp"

namespace FMA {
class Project;
namespace assem {
class BinaryCodeGeneratorScope;
class Instruction;
}
namespace plugin {

class BinaryGeneratorPluginAdapter {
public:
  BinaryGeneratorPluginAdapter(Project *project) : project(project) {}
  virtual ~BinaryGeneratorPluginAdapter() {}

  virtual bool supports(const std::string &name) const = 0;
  virtual bool generate(assem::BinaryCodeGeneratorScope *scope, assem::Instruction *instruct, const std::string &name) = 0;
  virtual bool finish() { return true; }

protected:
  Project *project;
};

}
}

#endif
