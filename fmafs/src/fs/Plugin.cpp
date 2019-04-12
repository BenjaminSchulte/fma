#include <fs/Plugin.hpp>
#include <fs/FileClass.hpp>
#include <fs/DirClass.hpp>
#include <fma/Project.hpp>
#include <fma/types/Module.hpp>
#include <fma/types/Class.hpp>
#include <fma/types/RootModule.hpp>

using namespace FMA;
using namespace FMA::fs;
using namespace FMA::types;

// ----------------------------------------------------------------------------
FsPlugin::FsPlugin(Project *project)
  : plugin::LanguagePlugin(project)
{
}

// ----------------------------------------------------------------------------
bool FsPlugin::initialize() {
  ModulePtr root(new Module("fs"));

  const RootModulePtr &_root = project->root();
  _root->setMember("fs", root);

  const ClassPtr &klass = _root->getMember("Class")->asClass();
  if (!klass) {
    return false;
  }

  FileClass::create(root, klass);
  DirClass::create(root, klass);

  return true;
}

// ----------------------------------------------------------------------------
void FsPlugin::release() {
}
