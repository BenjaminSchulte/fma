#include <fma/assem/BinaryCodeGeneratorScope.hpp>
#include <fma/linker/LinkerBlock.hpp>

using namespace FMA;
using namespace FMA::assem;
using namespace FMA::linker;

// ----------------------------------------------------------------------------
BinaryCodeGeneratorScope::BinaryCodeGeneratorScope(BinaryCodeGenerator *generator, LinkerBlock *target)
  : generator(generator)
  , linkerBlock(target)
{
}

// ----------------------------------------------------------------------------
BinaryCodeGeneratorScope::~BinaryCodeGeneratorScope() {
}

// ----------------------------------------------------------------------------
