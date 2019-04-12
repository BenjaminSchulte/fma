#include <fma/Project.hpp>
#include <fma/FileMap.hpp>
#include <fma/File.hpp>

#include <iostream>

using namespace FMA;

// ----------------------------------------------------------------------------
FileMap::FileMap(const Project *project)
  : project(project)
{
}

// ----------------------------------------------------------------------------
boost::filesystem::path FileMap::testFileNameAlternatives(const boost::filesystem::path &path, bool &valid) {
  valid = true;

  if (boost::filesystem::exists(path)) {
    // File exists

    if (boost::filesystem::is_regular_file(path)) {
      // File is a regular file which exists

      return path;
    } else if (boost::filesystem::is_directory(path)) {
      // Path is a directory - use index.fma file is provided

      boost::filesystem::path newPath = path;
      newPath /= "index.fma";

      if (boost::filesystem::exists(newPath) && boost::filesystem::is_regular_file(newPath)) {
        return newPath;
      } else {
        valid = false;
      }
    }
  } else {
    // File does not exist
    boost::filesystem::path newPath = path;
    newPath += ".fma";

    if (boost::filesystem::exists(newPath) && boost::filesystem::is_regular_file(newPath)) {
      return newPath;
    } else {
      valid = false;
    }
  }

  return path;
}

// ----------------------------------------------------------------------------
void FileMap::addIncludeDirectories(const IncludeDirectoryList &list) {
  for (const auto &item : list) {
    addIncludeDirectory(item);
  }
}

// ----------------------------------------------------------------------------
void FileMap::addIncludeDirectory(const std::string &name) {
  includeDirs.push_back(name);
}

// ----------------------------------------------------------------------------
FilePtr FileMap::resolve(const std::string &fileName, const FilePtr &relativeTo) {
  FilePtr file;

  if (relativeTo) {
    file = resolveVariant(boost::filesystem::absolute(fileName, relativeTo->path()));
  }

  if (!file) {
    file = resolveVariant(boost::filesystem::absolute(fileName));
  }

  if (!file) {
    for (const auto &dir : includeDirs) {
      file = resolveVariant(boost::filesystem::absolute(fileName, dir));

      if (file) {
        break;
      }
    }
  }

  if (!file) {
    project->log().error() << "Unable to resolve file " << fileName;
  }

  return file;
}

// ----------------------------------------------------------------------------
FilePtr FileMap::resolveVariant(const boost::filesystem::path &_path) {
  bool valid = false;
  boost::filesystem::path path = testFileNameAlternatives(_path, valid);
  if (!valid) {
    return FilePtr();
  }

  path = boost::filesystem::canonical(path);

  std::string pathAsString = path.string();
  auto file = files.find(pathAsString);
  if (file != files.end()) {
    project->log().trace() << "Reusing file from cache: " << path;
    return file->second;
  }

  project->log().trace() << "Found file: " << path;
  FilePtr fileObject = FilePtr(new File(pathAsString));
  files[pathAsString] = fileObject;
  return fileObject;
}
