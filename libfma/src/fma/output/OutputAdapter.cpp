#include <fma/linker/LinkerObject.hpp>
#include <fma/output/OutputAdapter.hpp>
#include <fma/Project.hpp>

using namespace FMA;
using namespace FMA::output;
using namespace FMA::plugin;
using namespace FMA::linker;

// ----------------------------------------------------------------------------
OutputAdapter::OutputAdapter(Project *project, LinkerObject *object)
  : project(project)
  , object(object)
{
}

// ----------------------------------------------------------------------------
OutputAdapter::~OutputAdapter() {
}

// ----------------------------------------------------------------------------
const MemorySymbolMap *OutputAdapter::getSymbolMap() {
  return project->getMemoryAdapter()->getSymbolMap();
}

// ----------------------------------------------------------------------------
