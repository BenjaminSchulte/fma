#include <schema/Plugin.hpp>
#include <fma/Project.hpp>
#include <fma/symbol/SymbolReference.hpp>
#include <fma/output/DynamicBuffer.hpp>
#include <memory/MemoryClassMembers.hpp>
#include <memory/MemoryClassMemberItem.hpp>
#include <fma/output/OutputAdapter.hpp>
#include <fma/linker/LinkerObject.hpp>
#include <fma/linker/LinkerBlock.hpp>
#include <fma/types/Class.hpp>
#include <fma/serialize/SerializerRegistry.hpp>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstring>

using namespace FMA;
using namespace FMA::output;
using namespace FMA::linker;
using namespace FMA::types;
using namespace FMA::memory;

// ----------------------------------------------------------------------------
OutputSchemaPlugin::OutputSchemaPlugin(Project *project)
  : OutputFileWriterPlugin(project)
{
}

// ----------------------------------------------------------------------------
bool OutputSchemaPlugin::initialize() {
  return true;
}

// ----------------------------------------------------------------------------
bool OutputSchemaPlugin::generate(OutputAdapter *adapter) {
  project->log().info() << "Building SNES schema file.";

  std::ostringstream os;
  os << "{";
  os << "\"types\":{\n";
  bool isFirstClass = true;

  const auto &klasses = project->serializer()->allClasses();
  FMA::serialize::TypeDeserializerClassMap::const_iterator it = klasses.begin();
  while (it != klasses.end()) {
    ClassPtr klass(it->second.lock());
    it++;

    if (!klass) {
      continue;
    }

    MemoryClassMembersPtr members(MemoryClassMembers::getClassMembers(klass));
    if (!members) {
      continue;
    }

    uint64_t size = members->getSize();
    if (size == 0) {
      continue;
    }

    os << std::string(isFirstClass ? "" : ",\n") + "\"" + klass->getFullName() + "\":{\"struct\":{\n";

    os << std::string("\"size\": ") + std::to_string(size) + ",\n";
    os << "\"members\": {\n";
    bool isFirstMember = true;
    for (const auto *member : members->allMembers()) {
      os << std::string(isFirstMember ? "" : ",\n") + "\"" + member->getName() + "\": {";
      os << "\"name\": \"" + member->getName() + "\",";
      os << "\"offset\": " + std::to_string(members->getOffsetOf(member->getName())) + ",";
      os << "\"type\": \"uint" + std::to_string(member->getItemSize() * 8) + "\"";
      os << "}";

      isFirstMember = false;
    }
    os << "}}}";

    isFirstClass = false;
  }

  os << "\n}";
  os << "}";

  BufferPtr buffer(adapter->openWithExtension(".schema.json"));
  if (!buffer) {
    project->log().warn() << "Unable to store schema.json file.";
    return true;
  }
  buffer->write(os.str());

  return true;
}

// ----------------------------------------------------------------------------
void OutputSchemaPlugin::release() {
}
