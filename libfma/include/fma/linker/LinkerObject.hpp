#ifndef __FMA_LINKER_LINKEROBJECT_H__
#define __FMA_LINKER_LINKEROBJECT_H__

#include <vector>

namespace FMA {
namespace linker {

class LinkerBlock;
typedef std::vector<LinkerBlock*> LinkerBlockList;

class LinkerObject {
public:
  LinkerObject();
  virtual ~LinkerObject();

  LinkerBlock *createBlock();
  void removeAllBlocks();

  inline const LinkerBlockList &getBlocks() const { return blocks; }

protected:
  LinkerBlockList blocks;
};

}
}

#endif
