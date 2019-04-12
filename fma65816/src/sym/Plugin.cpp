#include <sym/Plugin.hpp>
#include <fma/Project.hpp>
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
using namespace FMA65816::sym;

// ----------------------------------------------------------------------------
OutputPlugin::OutputPlugin(Project *project)
  : OutputFileWriterPlugin(project)
{
}

// ----------------------------------------------------------------------------
bool OutputPlugin::initialize() {
  return true;
}

// ----------------------------------------------------------------------------
bool OutputPlugin::generate(OutputAdapter *adapter) {
  project->log().info() << "Building SNES symbol file.";

  std::ostringstream os;
  os << "#SNES65816" << std::endl;
  os << "#Visit https://github.com/BenjaminSchulte/fma-snes65816/blob/master/docs/symbols.adoc for specifications." << std::endl;
  os << std::endl;
  os << "[SYMBOL]" << std::endl;

  auto symbols = adapter->getSymbolMap();
  for (const auto &name : symbols->getSymbolNames()) {
    uint64_t address = symbols->getResolved(name);

    os << std::hex << std::setw(2) << std::setfill('0') << ((address >> 16) & 0xFF) << ":";
    os << std::hex << std::setw(4) << std::setfill('0') << (address & 0xFFFF) << " ";
    os << name << " ANY 1" << std::endl;
  }

  os << std::endl;
  os << "[COMMAND]" << std::endl;
  for (const auto &cmd : symbols->getCommands()) {
    os << std::hex << std::setw(4) << std::setfill('0') << cmd.reference << " ";
    os << "\"" << cmd.command << "\"" << std::endl;
  }

  adapter->openWithExtension(".cpu.sym")->write(os.str());

  return true;
}

// ----------------------------------------------------------------------------
void OutputPlugin::release() {
}
