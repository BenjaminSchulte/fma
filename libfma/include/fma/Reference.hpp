#ifndef __FMA_REFERENCE_H__
#define __FMA_REFERENCE_H__

#include <string>
#include <memory>

namespace FMA {

class File;
typedef std::shared_ptr<class File> FilePtr;

/**
 * A file reference
 */
struct CodeReference {
  CodeReference()
    : file(), line(0), col(0), cachedName(NULL)
  {}
  
  CodeReference(const CodeReference &copy);
  CodeReference &operator=(const CodeReference &copy);

  CodeReference(const FilePtr &file, unsigned int line, unsigned int col)
    : file(file), line(line), col(col), cachedName(NULL)
  {}

  ~CodeReference();

  

  inline bool isValid() const {
    return line > 0 || col > 0;
  }

  operator const char*() { return asString(); }
  const char *asString();

  std::weak_ptr<File> file;
  unsigned int line;
  unsigned int col;
  
protected:
  char *cachedName;
};

}

#endif
