#include <fma/File.hpp>
#include <boost/filesystem.hpp>
#include <iostream>

using namespace FMA;

// ----------------------------------------------------------------------------
File::File(const std::string &fileName) 
  : fileName(fileName)
{
}

// ----------------------------------------------------------------------------
File::~File() {
}

// ----------------------------------------------------------------------------
std::string File::path() const {
  boost::filesystem::path path(fileName);
  return path.parent_path().string();
}

// ----------------------------------------------------------------------------

