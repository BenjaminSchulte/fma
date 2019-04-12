#ifndef __FMA_LINKER_LINKER_H__
#define __FMA_LINKER_LINKER_H__

#include <cstdint>
#include <vector>

namespace FMA {
class Project;
namespace linker {

class LinkerObject;

class Linker {
public:
  Linker(Project *project, LinkerObject *object);
  virtual ~Linker();

  inline Project *getProject() const { return project; }
  inline LinkerObject *getLinkerObject() const { return object; }

  bool link();

protected:
  Project *project;
  LinkerObject *object;
};

}
}

#endif
