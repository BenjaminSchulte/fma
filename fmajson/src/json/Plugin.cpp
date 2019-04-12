#include <json/Plugin.hpp>
#include <json/JsonClass.hpp>
#include <fma/Project.hpp>
#include <fma/types/Module.hpp>
#include <fma/types/Class.hpp>
#include <fma/types/RootModule.hpp>

using namespace FMA;
using namespace FMA::json;
using namespace FMA::types;

// ----------------------------------------------------------------------------
JsonPlugin::JsonPlugin(Project *project)
  : plugin::LanguagePlugin(project)
{
}

// ----------------------------------------------------------------------------
bool JsonPlugin::initialize() {
  ModulePtr root(new Module("fmaJSON"));

  const RootModulePtr &_root = project->root();
  _root->setMember("fmaJSON", root);

  const ClassPtr &klass = _root->getMember("Class")->asClass();
  if (!klass) {
    return false;
  }

  JsonClass::create(root, klass);

  return true;
}

// ----------------------------------------------------------------------------
void JsonPlugin::release() {
}
