#ifndef __FMA_FILE_H__
#define __FMA_FILE_H__

#include <string>
#include <memory>

namespace FMA {
typedef std::shared_ptr<class File> FilePtr;

namespace ast {
typedef std::shared_ptr<class Node> NodePtr;
}

/**
 * A file
 */
class File {
public:
  File(const std::string &fileName);
  virtual ~File();

  inline const std::string &absoluteFileName() const {
    return fileName;
  }

  std::string path() const;
  inline void setRootNode(const ast::NodePtr &root) {
    this->root = root;
  }

  inline bool hasRootNode() const {
    return !!root;
  }

protected:
  std::string fileName;
  ast::NodePtr root;
};

}

#endif
