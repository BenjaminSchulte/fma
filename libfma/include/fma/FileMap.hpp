#ifndef __FMA_FILEMAP_H__
#define __FMA_FILEMAP_H__

#include <map>
#include <string>
#include <memory>
#include <boost/filesystem.hpp>

namespace FMA {

class File;
typedef std::shared_ptr<File> FilePtr;
typedef std::vector<std::string > IncludeDirectoryList;

class FileMap {
public:
  FileMap(const Project *project);

  FilePtr resolve(const std::string &fileName, const FilePtr &relativeTo=FilePtr());
  boost::filesystem::path testFileNameAlternatives(const boost::filesystem::path &fileName, bool &valid);

  void addIncludeDirectories(const IncludeDirectoryList &list);
  void addIncludeDirectory(const std::string &name);

protected:
  const Project *project;

  std::map<std::string, FilePtr> files;
  IncludeDirectoryList includeDirs;

  FilePtr resolveVariant(const boost::filesystem::path &path);
};

}

#endif