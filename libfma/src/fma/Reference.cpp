#include <fma/Reference.hpp>
#include <fma/File.hpp>
#include <sstream>
#include <string.h>

using namespace FMA;

// ----------------------------------------------------------------------------
CodeReference::CodeReference(const CodeReference &copy) {
  file = copy.file;
  line = copy.line;
  col = copy.col;

  cachedName = NULL;
}

// ----------------------------------------------------------------------------
CodeReference::~CodeReference() {
  if (cachedName) {
    free(cachedName);
  }
}

// ----------------------------------------------------------------------------
const char *CodeReference::asString() {
  if (cachedName) {
    return cachedName;
  }

  std::ostringstream os;
  FilePtr f = file.lock();
  if (f) {
    os << f->absoluteFileName() << ":";
  }
  os << line << ":" << col << ": ";

  cachedName = strdup(os.str().c_str());
  return cachedName;
}
