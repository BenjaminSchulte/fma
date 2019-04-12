#ifndef __FMA_ASM_BINARYCODEGENERATORSCOPE_H__
#define __FMA_ASM_BINARYCODEGENERATORSCOPE_H__

#include "BinaryCodeGenerator.hpp"

namespace FMA {
namespace linker {
class LinkerBlock;
}
namespace assem {

class BinaryCodeGeneratorScope {
public:
  BinaryCodeGeneratorScope(BinaryCodeGenerator *generator, linker::LinkerBlock *linkerBlock);
  virtual ~BinaryCodeGeneratorScope();

  inline BinaryCodeGenerator *getGenerator() const { return generator; }
  inline Project *getProject() const { return generator->getProject(); }
  inline linker::LinkerObject *getLinkerObject() const { return generator->getLinkerObject(); }
  inline linker::LinkerBlock *getLinkerBlock() const { return linkerBlock; }

protected:
  BinaryCodeGenerator *generator;
  linker::LinkerBlock *linkerBlock;
};

}
}

#endif