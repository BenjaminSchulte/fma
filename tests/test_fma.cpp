#include <fma/Project.hpp>
#include <fma/Parser.hpp>
#include <fma/core/Plugin.hpp>
#include <fma/interpret/Interpreter.hpp>
#include <fma/types/RootModule.hpp>
#include <iostream>

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
