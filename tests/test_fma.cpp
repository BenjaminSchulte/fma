#include <fma/Project.hpp>
#include <fma/Parser.hpp>
#include <fma/core/Plugin.hpp>
#include <fma/interpret/Interpreter.hpp>
#include <fma/types/RootModule.hpp>
#include <iostream>

FMA::plugin::PluginList fmamemory_fmaGetPlugins(FMA::Project *project);

int main(int argc, char **argv) {
  if (argc < 2) {
    return 1;
  }

  FMA::Project project;
  FMA::Parser parser(&project);

  FMA::core::CorePlugin plugin(&project);
  if (!plugin.initialize()) {
    project.log().error() << "Could not initialize core plugin";
    return 1;
  }

  std::shared_ptr<FMA::plugin::MemoryManagerPlugin> memoryManagerPlugin;
  for (const auto &plugin : fmamemory_fmaGetPlugins(&project)) {
    if (!plugin->initialize()) {
      project.log().error() << "Unable to initialize " << plugin->getName();
      return 1;
    }

    if (plugin->getPluginType() == FMA::plugin::TYPE_MEMORY_MANAGER) {
      memoryManagerPlugin = std::dynamic_pointer_cast<FMA::plugin::MemoryManagerPlugin>(plugin);
    }
  }

  if (memoryManagerPlugin) {
    project.setMemoryAdapter(memoryManagerPlugin->createAdapter());
  }

  if (!parser.parseFile(argv[1])) {
    project.log().error() << "Failed to parse file";
    
    for(auto const& error: parser.getErrors()) {
      project.log().error() << error.file.line << ":" << error.file.col << ": " << error.message;
    }

    plugin.release();
    return 1;
  }

  FMA::interpret::Interpreter interpreter(&project);
  interpreter.execute(parser.getResult());

  // project.root()->dump("");
  
  return project.hasErrors() ? 1 : 0;
}
