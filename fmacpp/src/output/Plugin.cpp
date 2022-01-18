#include <output/Plugin.hpp>
#include <binary/CodeSection.hpp>
#include <fma/Project.hpp>
#include <fma/output/DynamicBuffer.hpp>
#include <fma/output/OutputAdapter.hpp>
#include <fma/linker/LinkerObject.hpp>
#include <fma/linker/LinkerBlock.hpp>
#include <fma/symbol/SymbolReference.hpp>
#include <string>
#include <sstream>
#include <iostream>

using namespace FMA;
using namespace FMA::output;
using namespace FMA::linker;
using namespace FMACPP::binary;
using namespace FMACPP::output;

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
  project->log().info() << "Building SNES C++ file.";

  BufferPtr file = adapter->open();
  if (!file) {
    return false;
  }

  std::stringstream sout;
  sout << "#include <fmacppdriver.hpp>\n";

  // Registers all data blocks
  std::stringstream sdeclare;
  std::stringstream scode;
  std::stringstream scoderegister;
  std::stringstream sdata;
  std::stringstream sdataregister;
  for (const auto *block : adapter->getObject()->getBlocks()) {
    if (block->getSize() == 0) {
      project->log().warn() << "Block is empty";
      continue;
    }

    const std::map<std::string, FMA::linker::LinkerBlockUserDataPtr> &map = block->getUserData();
    std::map<std::string, FMA::linker::LinkerBlockUserDataPtr>::const_iterator it = map.find("FMACPP");
    if (it == map.end()) {
      project->log().warn() << "Block has no SNESCPP object";
      continue;
    }

    if (it->second->typeId() != LinkerBlockCodeSection::TypeId()) {
      project->log().error() << "Invalid type ID for object: " << it->second->typeId();
      continue;
    }
    
    std::shared_ptr<LinkerBlockCodeSection> pos(std::dynamic_pointer_cast<LinkerBlockCodeSection>(it->second));
    if (!pos) {
      project->log().error() << "Unable to cast to LinkerBlockCodeSection";
      continue;
    }

    for (const LinkerBlockCodeSectionPart &part : pos->parts) {
      uint64_t snesAddress = project->getMemoryAdapter()->getSymbolMap()->getResolved(part.label);
      //uint64_t snesAddress = block->getPlacement()->asLongAddress();
      if (part.hasCode) {
        scode << "\n// " + part.label;
        sdeclare << "int32_t __routine_" << snesAddress << "();\n";
        scode << "\nint32_t __routine_" << snesAddress << "() {\n";
        for (const auto &code : part.code) {
          switch (code.type) {
          case LinkerBlockCodeSectionCode::CODE:
            scode << code.code;
            break;
          case LinkerBlockCodeSectionCode::SYMBOL:
            scode << project->getMemoryAdapter()->getSymbolMap()->getResolved(code.ref->asString());
            break;
          }
        }
        scode << "}\n";

        scoderegister << "    case " << snesAddress << ": return __routine_" << snesAddress << "();\n";
      } else {
        sdata << "\nconst char __static_data_" << snesAddress << "[] = {\n  ";

        char* data = (char*)block->getData();
        for (uint32_t i=0; i<block->getSize(); i++) {
          if (i > 0 && (i % 16) == 0) { sdata << ", \n  "; }
          else if (i > 0) { sdata << ", "; }
          
          sdata << (int)data[i];
        }
        sdata << "\n};\n";

        sdataregister << "  else if (address >= " << snesAddress << " && address < " << (snesAddress + block->getSize()) << ") { return __static_data_" << snesAddress << "[address - " << snesAddress << "]; }\n";
      }
    }
  }

  // Private
  sout << "\nnamespace {\n";
  sout << sdeclare.str();
  sout << sdata.str();
  sout << scode.str();
  sout << "}\n\n";

  // Global
  sout << "uint8_t FmaCpp::romRead(uint32_t address) {\n";
  sout << "  if (false) {}\n";
  sout << sdataregister.str();
  sout << "  printLog(std::string(\"Invalid ROM access at address \") + std::to_string(address), LOG_ERROR);\n";
  sout << "  return 255;\n";
  sout << "}\n\n";

  sout << "int32_t FmaCpp::gameResume(int32_t address) {\n";
  sout << "  setPC(address);\n";
  sout << "  switch(address) {\n";
  sout << scoderegister.str();
  sout << "    default: return -1;\n";
  sout << "  }\n";
  sout << "}\n\n";

  if (!file->write(sout.str())) {
    project->log().error() << "Unable to write data to file buffer.";
    return false;
  }

  return true;
}

// ----------------------------------------------------------------------------
void OutputPlugin::release() {
}
