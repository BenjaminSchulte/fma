#include <csv/Plugin.hpp>
#include <csv/CsvClass.hpp>
#include <fma/Project.hpp>
#include <fma/types/Module.hpp>
#include <fma/types/Class.hpp>
#include <fma/types/RootModule.hpp>

using namespace FMA;
using namespace FMA::csv;
using namespace FMA::types;

// ----------------------------------------------------------------------------
CsvPlugin::CsvPlugin(Project *project)
  : plugin::LanguagePlugin(project)
{
}

// ----------------------------------------------------------------------------
bool CsvPlugin::initialize() {
  ModulePtr root(new Module("fmaCSV"));

  const RootModulePtr &_root = project->root();
  _root->setMember("fmaCSV", root);

  const ClassPtr &klass = _root->getMember("Class")->asClass();
  if (!klass) {
    return false;
  }

  CsvClass::create(root, klass);

  return true;
}

// ----------------------------------------------------------------------------
void CsvPlugin::release() {
}
