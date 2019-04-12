#include <Project.hpp>
#include <fma/Project.hpp>
#include <fma/FileMap.hpp>
#include <fma/core/Plugin.hpp>
#include <iostream>

FMA::plugin::PluginList fma65816_fmaGetPlugins(FMA::Project *project);
FMA::plugin::PluginList fmamemory_fmaGetPlugins(FMA::Project *project);
FMA::plugin::PluginList fmacsv_fmaGetPlugins(FMA::Project *project);
FMA::plugin::PluginList fmajson_fmaGetPlugins(FMA::Project *project);
FMA::plugin::PluginList fmafs_fmaGetPlugins(FMA::Project *project);
#ifdef WITH_FMAIMAGE
FMA::plugin::PluginList fmaimage_fmaGetPlugins(FMA::Project *project);
#endif

// ----------------------------------------------------------------------------
Project::Project() {
  project = new FMA::Project();
}

// ----------------------------------------------------------------------------
Project::~Project() {
  unloadPlugins();
  delete project;
}

// ----------------------------------------------------------------------------
Nan::MaybeLocal<v8::Function> Project::CREATE() {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Project").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  
  Nan::SetPrototypeMethod(tpl, "addIncludeDirectory", AddIncludeDirectory);

  constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
  return Nan::GetFunction(tpl);
}

// ----------------------------------------------------------------------------
NAN_METHOD(Project::AddIncludeDirectory) {
  FMA::Project *project = Nan::ObjectWrap::Unwrap<Project>(info.Holder())->project;

  if (info.Length() != 1) {
    return Nan::ThrowError(Nan::New("Missing argument for include dir").ToLocalChecked());
  }

  if (!info[0]->IsString()) {
    return Nan::ThrowError(Nan::New("Argument must be a string").ToLocalChecked());
  }

  v8::String::Utf8Value value(info[0]->ToString());
  project->getFiles()->addIncludeDirectory(*value);

  info.GetReturnValue().Set(info.This());
}

// ----------------------------------------------------------------------------
NAN_METHOD(Project::New) {
  if (info.Length() != 1) {
    return Nan::ThrowError(Nan::New("Missing language for project").ToLocalChecked());
  }

  if (!info[0]->IsString()) {
    return Nan::ThrowError(Nan::New("Argument must be a string").ToLocalChecked());
  }
  
  v8::String::Utf8Value value(info[0]->ToString());
  std::string language(*value);

  Project *obj = new Project();
  if (!obj->loadPlugins()) {
    return Nan::ThrowError(Nan::New("Unable to load core plugins").ToLocalChecked());
  }
  if (!obj->loadLanguagePlugins(language)) {
    return Nan::ThrowError(Nan::New("Unsupported language").ToLocalChecked());
  }

  obj->Wrap(info.This());
  info.GetReturnValue().Set(info.This());
}

// ----------------------------------------------------------------------------
bool Project::loadPlugin(const PluginPtr &plugin) {
  project->log().debug() << "Loading plugin: " << plugin->getName();

  if (!plugin->initialize()) {
    project->log().error() << "Could not initialize plugin: " << plugin->getName();
    return false;
  }

  project->log().debug() << "Plugin has been loaded: " << plugin->getName();
  plugins.push_back(plugin);
  return true;
}

// ----------------------------------------------------------------------------
bool Project::loadLanguagePlugins(const std::string &language) {
  if (language == "65816") {
    plugins = fma65816_fmaGetPlugins(project);
  } else {
    project->log().fatal() << "Unsupported language: " << language;
    return false;
  }

  for (auto &plugin : plugins) {
    if (!loadPlugin(plugin)) {
      return false;
    }

    if (plugin->getPluginType() == FMA::plugin::TYPE_BINARY_BUILDER) {
      std::cout << "OK 1!!!" << std::endl;
      project->addByteCodeGenerator(std::dynamic_pointer_cast<FMA::plugin::BinaryGeneratorPlugin>(plugin)->createAdapter());
    }

  std::cout << "OK 2!!!" << std::endl;

    if (plugin->getPluginType() == FMA::plugin::TYPE_OUTPUT_FILE_BUILDER) {
      outputFileWriters.push_back(std::dynamic_pointer_cast<FMA::plugin::OutputFileWriterPlugin>(plugin));
    }

  std::cout << "OK 3!!!" << std::endl;
  }

  return true;
}

// ----------------------------------------------------------------------------
bool Project::loadPlugins(const PluginList &all) {
  for (auto &plugin : all) {
    if (!loadPlugin(plugin)) {
      return false;
    }
  }

  return true;
}

// ----------------------------------------------------------------------------
bool Project::loadMemoryPlugins() {
  for (auto &plugin : fmamemory_fmaGetPlugins(project)) {
    if (!loadPlugin(plugin)) {
      return false;
    }

    if (plugin->getPluginType() == FMA::plugin::TYPE_MEMORY_MANAGER) {
      project->setMemoryAdapter(std::dynamic_pointer_cast<FMA::plugin::MemoryManagerPlugin>(plugin)->createAdapter());
    }
  }

  return true;
}

// ----------------------------------------------------------------------------
bool Project::loadPlugins() {
  if (!loadPlugin(PluginPtr(new FMA::core::CorePlugin(project)))) {
    return false;
  }

  if (!loadMemoryPlugins()) {
    return false;
  }

  if (!loadPlugins(fmacsv_fmaGetPlugins(project))) {
    return false;
  }

  if (!loadPlugins(fmajson_fmaGetPlugins(project))) {
    return false;
  }

  if (!loadPlugins(fmafs_fmaGetPlugins(project))) {
    return false;
  }

# ifdef WITH_FMAIMAGE
    if (!loadPlugins(fmaimage_fmaGetPlugins(project))) {
      return false;
    }
# endif

  return true;
}

// ----------------------------------------------------------------------------
void Project::unloadPlugins() {
  outputFileWriters.clear();

  for (auto &plugin : plugins) {
    project->log().trace() << "Unloading plugin: " << plugin->getName();

    if (plugin->getPluginType() == FMA::plugin::TYPE_BINARY_BUILDER) {
      for (auto &generator : project->getByteCodeGenerators()) {
        std::dynamic_pointer_cast<FMA::plugin::BinaryGeneratorPlugin>(plugin)->releaseAdapter(generator);
      }
      project->removeAllByteCodeGenerators();
    }

    if (plugin->getPluginType() == FMA::plugin::TYPE_MEMORY_MANAGER) {
      std::dynamic_pointer_cast<FMA::plugin::MemoryManagerPlugin>(plugin)->releaseAdapter(project->getMemoryAdapter());
      project->setMemoryAdapter(NULL);
    }

    plugin->release();
  }

  plugins.clear();
}
