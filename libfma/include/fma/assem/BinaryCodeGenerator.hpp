#ifndef __FMA_ASM_BINARYCODEGENERATOR_H__
#define __FMA_ASM_BINARYCODEGENERATOR_H__

#include <string>
#include <vector>
#include "../plugin/MemoryPluginAdapter.hpp"

namespace FMA {
class Project;
namespace linker {
class LinkerObject;
}
namespace assem {

class BinaryCodeGenerator {
public:
  BinaryCodeGenerator(class Project*, linker::LinkerObject *object);
  virtual ~BinaryCodeGenerator();

  void add(plugin::MemoryBlock *block);
  bool generate();
  bool generate(plugin::MemoryBlock *block);

  inline Project *getProject() const { return project; }
  inline linker::LinkerObject *getLinkerObject() const { return object; }

protected:
  Project *project;
  linker::LinkerObject *object;
  plugin::MemoryBlockList blocks;
};
}
}

#endif