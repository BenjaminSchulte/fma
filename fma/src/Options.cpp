#include <fma/Version.hpp>
#include <vector>
#include <iostream>
#include <sstream>

#include "Options.hpp"

namespace po = boost::program_options;

// ----------------------------------------------------------------------------
bool Options::parse(int argc, char **argv) {
  std::ostringstream os;
  os << "Famicom Macro Assembler " << FMA::getLibraryVersion();

  po::options_description desc(os.str());
  desc.add_options()
    ("help,h", "shows this help")
    ("language,l", po::value<std::string>(), "the assembler language to use")
    ("include-symbol,S", po::value<std::vector<std::string>>(), "add symbol files")
    ("input-files", po::value<std::vector<std::string>>(), "the input files")
    ("cwd", po::value<std::string>(), "change the current working directory")
    ("include-dir,I", po::value<std::vector<std::string>>(), "add include directories")
    ("output,o", po::value<std::string>()->default_value("a.bin"), "the output binary file")
    ("no-output", po::value<bool>()->default_value(false), "prevents generating a binary file")
    ("generator", po::value<std::string>(), "selects the binary generator")
  ;

  po::positional_options_description p;
  p.add("input-files", -1);

  po::store(
    po::command_line_parser(argc, argv)
      .options(desc)
      .positional(p)
      .run(),
    vm
  );
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return false;
  }

  if (!vm.count("language")) {
    std::cerr << "Error: Missing language. Use -l <language>" << std::endl;
    return false;
  }
  
  if (!vm.count("input-files")) {
    std::cerr << "Error: Missing input files for compiler" << std::endl;
    return false;
  }

  return true;
}
