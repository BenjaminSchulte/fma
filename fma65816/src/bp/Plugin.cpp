#include <bp/Plugin.hpp>
#include <fma/Project.hpp>
#include <fma/symbol/SymbolReference.hpp>
#include <fma/output/DynamicBuffer.hpp>
#include <fma/output/OutputAdapter.hpp>
#include <fma/linker/LinkerObject.hpp>
#include <fma/linker/LinkerBlock.hpp>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstring>

using namespace FMA;
using namespace FMA::output;
using namespace FMA::linker;
using namespace FMA65816::bp;

// ----------------------------------------------------------------------------
OutputPlugin::OutputPlugin(Project *project)
  : OutputFileWriterPlugin(project)
{
}

// ----------------------------------------------------------------------------
bool OutputPlugin::initialize() {
  return true;
}

//000000:rwx:cpu

// ----------------------------------------------------------------------------
bool OutputPlugin::generate(OutputAdapter *adapter) {
  project->log().info() << "Building SNES breakpoint file.";

  std::ostringstream os;

  auto symbols = adapter->getSymbolMap();
  for (const auto &bp : symbols->getBreakpoints()) {
    uint64_t address = symbols->getResolved(bp.reference->asString());
    os << std::hex << std::setw(6) << std::setfill('0') << (address & 0xFFFFFF) << ":x" << (bp.notifyOnly ? "n" : "") << ":cpu#" << bp.comment << std::endl;
  }

  adapter->openWithExtension(".bp")->write(os.str());

  return true;
}

// ----------------------------------------------------------------------------
void OutputPlugin::release() {
}
