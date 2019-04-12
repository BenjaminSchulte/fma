#ifndef __FMA_PARSER__H__
#define __FMA_PARSER__H__

#include <vector>
#include <memory>
#include "Error.hpp"

namespace FMA {
namespace ast {
typedef std::shared_ptr<class Node> NodePtr;
}

class File;

class Parser {
public:
  Parser(class Project *project);
  virtual ~Parser();

  bool parseString(const char *string);
  bool parseFile(const char *fileName, const FilePtr &relativeTo=FilePtr());

  CodeReference reference(unsigned int line, unsigned int column) const;
  void error(unsigned int line, unsigned int column, const std::string &message);

  const ast::NodePtr &getResult() const;
  inline const std::vector<Error> &getErrors() const {
    return errors;
  }

protected:
  Project *project;
  FilePtr currentFile;

  ast::NodePtr result;

  std::vector<Error> errors;
};

}

#endif
