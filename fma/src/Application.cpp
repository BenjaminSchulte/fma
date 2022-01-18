#include <boost/filesystem.hpp>
#include <fma/core/Plugin.hpp>
#include <fma/plugin/Plugin.hpp>
#include <fma/output/FileOutputAdapter.hpp>
#include <fma/interpret/Interpreter.hpp>
#include <fma/linker/Linker.hpp>
#include <fma/linker/LinkerObjectDeserializer.hpp>
#include <fma/assem/BinaryCodeGenerator.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/Parser.hpp>
#include <fma/FileMap.hpp>
#include <iostream>
#include "Application.hpp"

//FMA::plugin::PluginList fmacpp_fmaGetPlugins(FMA::Project *project);
FMA::plugin::PluginList fma65816_fmaGetPlugins(FMA::Project *project);
FMA::plugin::PluginList fmaspc_fmaGetPlugins(FMA::Project *project);
FMA::plugin::PluginList fmamemory_fmaGetPlugins(FMA::Project *project);
FMA::plugin::PluginList fmacsv_fmaGetPlugins(FMA::Project *project);
FMA::plugin::PluginList fmajson_fmaGetPlugins(FMA::Project *project);
FMA::plugin::PluginList fmafs_fmaGetPlugins(FMA::Project *project);
FMA::plugin::PluginList fmasuperfx_fmaGetPlugins(FMA::Project *project);
FMA::plugin::PluginList fmacpp_fmaGetPlugins(FMA::Project *project);
#ifdef WITH_FMAIMAGE
FMA::plugin::PluginList fmaimage_fmaGetPlugins(FMA::Project *project);
#endif

// ----------------------------------------------------------------------------
Application::Application() {
}

// ----------------------------------------------------------------------------
Application::~Application() {
  unloadPlugins();
}

// ----------------------------------------------------------------------------
bool Application::loadPlugin(const PluginPtr &plugin) {
  project.log().debug() << "Loading plugin: " << plugin->getName();

  if (!plugin->initialize()) {
    project.log().error() << "Could not initialize plugin: " << plugin->getName();
    return false;
  }

  project.log().debug() << "Plugin has been loaded: " << plugin->getName();
  plugins.push_back(plugin);
  return true;
}

// ----------------------------------------------------------------------------
bool Application::loadLanguagePlugins() {
  const std::string &language = options.args()["language"].as<std::string>();
  PluginList plugins;

  if (language == "65816") {
    plugins = fma65816_fmaGetPlugins(&project);
  } else if (language == "spc" || language == "SPC") {
    plugins = fmaspc_fmaGetPlugins(&project);
  } else {
    project.log().fatal() << "Unsupported language: " << language;
    return false;
  }

  for (auto &plugin : plugins) {
    if (!loadPlugin(plugin)) {
      return false;
    }

    if (target == "binary") {
      if (plugin->getPluginType() == FMA::plugin::TYPE_BINARY_BUILDER) {
        project.addByteCodeGenerator(std::dynamic_pointer_cast<FMA::plugin::BinaryGeneratorPlugin>(plugin)->createAdapter());
      }

      if (plugin->getPluginType() == FMA::plugin::TYPE_OUTPUT_FILE_BUILDER) {
        outputFileWriters.push_back(std::dynamic_pointer_cast<FMA::plugin::OutputFileWriterPlugin>(plugin));
      }
    }
  }

  return true;
}

// ----------------------------------------------------------------------------
bool Application::loadCppPlugins() {
  PluginList plugins;
  plugins = fmacpp_fmaGetPlugins(&project);

  for (auto &plugin : plugins) {
    if (!loadPlugin(plugin)) {
      return false;
    }

    if (plugin->getPluginType() == FMA::plugin::TYPE_BINARY_BUILDER) {
      project.removeAllByteCodeGenerators();
      project.addByteCodeGenerator(std::dynamic_pointer_cast<FMA::plugin::BinaryGeneratorPlugin>(plugin)->createAdapter());
    }
    if (plugin->getPluginType() == FMA::plugin::TYPE_OUTPUT_FILE_BUILDER) {
      outputFileWriters.push_back(std::dynamic_pointer_cast<FMA::plugin::OutputFileWriterPlugin>(plugin));
    }
  }

  return true;
}
// ----------------------------------------------------------------------------
bool Application::loadPlugins(const PluginList &all) {
  for (auto &plugin : all) {
    if (!loadPlugin(plugin)) {
      return false;
    }
  }

  return true;
}

// ----------------------------------------------------------------------------
bool Application::loadMemoryPlugins() {
  for (auto &plugin : fmamemory_fmaGetPlugins(&project)) {
    if (!loadPlugin(plugin)) {
      return false;
    }

    if (plugin->getPluginType() == FMA::plugin::TYPE_MEMORY_MANAGER) {
      project.setMemoryAdapter(std::dynamic_pointer_cast<FMA::plugin::MemoryManagerPlugin>(plugin)->createAdapter());
    }

    if (target == "binary") {
      if (plugin->getPluginType() == FMA::plugin::TYPE_OUTPUT_FILE_BUILDER) {
        outputFileWriters.push_back(std::dynamic_pointer_cast<FMA::plugin::OutputFileWriterPlugin>(plugin));
      }
    }
  }

  return true;
}

// ----------------------------------------------------------------------------
bool Application::loadPlugins() {
  if (!options.args()["generator"].empty()) {
    target = options.args()["generator"].as<std::string>();
  }

  project.setTargetName(target);

  if (!loadPlugin(PluginPtr(new FMA::core::CorePlugin(&project)))) {
    return false;
  }

  if (!loadMemoryPlugins()) {
    return false;
  }

  if (!loadPlugins(fmacsv_fmaGetPlugins(&project))) {
    return false;
  }

  if (!loadPlugins(fmajson_fmaGetPlugins(&project))) {
    return false;
  }

  if (!loadPlugins(fmafs_fmaGetPlugins(&project))) {
    return false;
  }

  if (!loadPlugins(fmasuperfx_fmaGetPlugins(&project))) {
    return false;
  }

# ifdef WITH_FMAIMAGE
    if (!loadPlugins(fmaimage_fmaGetPlugins(&project))) {
      return false;
    }
# endif

  if (!loadLanguagePlugins()) {
    return false;
  }

  if (target == "c++") {
    if (!loadCppPlugins()) {
      return false;
    }
  }

  return true;
}

// ----------------------------------------------------------------------------
void Application::unloadPlugins() {
  outputFileWriters.clear();

  for (auto &plugin : plugins) {
    project.log().trace() << "Unloading plugin: " << plugin->getName();

    if (plugin->getPluginType() == FMA::plugin::TYPE_BINARY_BUILDER) {
      for (auto &generator : project.getByteCodeGenerators()) {
        std::dynamic_pointer_cast<FMA::plugin::BinaryGeneratorPlugin>(plugin)->releaseAdapter(generator);
      }
      project.removeAllByteCodeGenerators();
    }

    if (plugin->getPluginType() == FMA::plugin::TYPE_MEMORY_MANAGER) {
      std::dynamic_pointer_cast<FMA::plugin::MemoryManagerPlugin>(plugin)->releaseAdapter(project.getMemoryAdapter());
      project.setMemoryAdapter(NULL);
    }

    plugin->release();
  }

  plugins.clear();
}

// ----------------------------------------------------------------------------
bool Application::parseAndInterpretFile(const std::string &file) {
  FMA::Parser parser(&project);
  FMA::interpret::Interpreter interpreter(&project);

  project.log().debug() << "Parsing input file: " << file;
  if (!parser.parseFile(file.c_str())) {
    project.log().error() << "Failed to parse input file: " << file;
    for(auto const& error: parser.getErrors()) {
      project.log().error() << error.file.line << ":" << error.file.col << ": " << error.message;
    }

    return false;
  }

  project.log().debug() << "Interpreting input file: " << file;
  interpreter.execute(parser.getResult());

  if (project.log().hasErrors()) {
    project.log().error() << "Cancel processing due to previous errors";
    return false;
  }

  project.log().debug() << "Done processing input file: " << file;
  return true;
}

// ----------------------------------------------------------------------------
bool Application::parseAndInterpretFiles() {
  for (auto &file : options.args()["input-files"].as<std::vector<std::string> >()) {
    if (!parseAndInterpretFile(file)) {
      return false;
    }
  }

  return true;
}

// ----------------------------------------------------------------------------
FMA::linker::LinkerObject *Application::buildBinaryCode() {
  project.log().info() << "Building bytecode.";

  FMA::linker::LinkerObject *object = new FMA::linker::LinkerObject();
  FMA::assem::BinaryCodeGenerator generator(&project, object);

  for (auto &block : project.getMemoryAdapter()->getBlocks()) {
    generator.add(block);
  }

  if (!generator.generate()) {
    return NULL;
  };

  return object;
}

// ----------------------------------------------------------------------------
bool Application::linkExternObjectFiles(FMA::linker::LinkerObject *object) {
  if (options.args()["include-symbol"].empty()) {
    return true;
  }

  project.log().info() << "Linking additional symbol files.";
  for (const std::string &file : options.args()["include-symbol"].as<std::vector<std::string>>()) {
    FMA::linker::LinkerObjectDeserializer deserializer(&project.log(), project.getMemoryAdapter(), object);
    if (!deserializer.deserialize(file.c_str())) {
      return false;
    }
  }

  return true;
}

// ----------------------------------------------------------------------------
bool Application::link(FMA::linker::LinkerObject *object) {
  project.log().info() << "Linking project.";

  FMA::linker::Linker linker(&project, object);
  if (!linker.link()) {
    project.log().error() << "Failed to link project.";
    return false;
  }

  return true;
}

// ----------------------------------------------------------------------------
bool Application::generateRom(FMA::linker::LinkerObject *object) {
  if (!outputFileWriters.size()) {
    project.log().error() << "No output file generator plugin has been loaded.";
    return false;
  }
  
  std::string fileName = options.args()["output"].as<std::string>();
  boost::filesystem::path file(fileName);
  boost::filesystem::path fullPath(boost::filesystem::absolute(file));
  boost::filesystem::path pathOnly(fullPath.parent_path());
  boost::filesystem::path nameOnly(fullPath.filename());

  FMA::output::FileOutputAdapter adapter(&project, object, pathOnly.string(), nameOnly.string());

  for (const auto &plugin : outputFileWriters) {
    if (!plugin->generate(&adapter)) {
      return false;
    }
  }

  return true;
}

// ----------------------------------------------------------------------------
int Application::runAfterInitialize() {
  if (!parseAndInterpretFiles()) {
    return 1;
  }

  if (!options.args()["no-output"].as<bool>()) {
    FMA::linker::LinkerObject *object = buildBinaryCode();
    if (!object) {
      return 1;
    }

    if (!linkExternObjectFiles(object)) {
      delete object;
      return 1;
    }

    if (!link(object)) {
      delete object;
      return 1;
    }

    if (!generateRom(object)) {
      delete object;
      return 1;
    }

    delete object;
  }

  return 0;
}

// ----------------------------------------------------------------------------
bool Application::loadOptions() {
  if (!options.args()["include-dir"].empty()) {
    project.getFiles()->addIncludeDirectories(options.args()["include-dir"].as<std::vector<std::string> >());
  }

  if (!options.args()["cwd"].empty()) {
    boost::filesystem::current_path(options.args()["cwd"].as<std::string>());
  }

  return true;
}

// ----------------------------------------------------------------------------
int Application::run(int argc, char **argv) {
  if (!options.parse(argc, argv)) {
    return 1;
  }

  if (!loadOptions()) {
    return 1;
  }

  if (!loadPlugins()) {
    unloadPlugins();
    return 1;
  }

  int result = runAfterInitialize();

  unloadPlugins();

  return result;
}

