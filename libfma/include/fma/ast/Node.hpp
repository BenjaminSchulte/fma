#ifndef __FMA_NODE_H__
#define __FMA_NODE_H__

#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <stdint.h>
#include "../Reference.hpp"

namespace FMA {
namespace interpret {
  class Result;
  class BaseContext;
  typedef std::shared_ptr<Result> ResultPtr;
  typedef std::shared_ptr<BaseContext> ContextPtr;
}
namespace ast {

typedef std::shared_ptr<class Node> NodePtr;
typedef std::shared_ptr<class Identifier> IdentifierPtr;
typedef std::shared_ptr<class Statement> StatementPtr;
typedef std::shared_ptr<class Expression> ExpressionPtr;

class Expression;
class MacroParameter;
typedef std::shared_ptr<MacroParameter> MacroParameterPtr;
typedef std::vector<MacroParameterPtr> MacroParameterList;

class Node {
public:
  Node() {}
  virtual ~Node() {}

  virtual interpret::ResultPtr execute(const interpret::ContextPtr &context) const;

  virtual std::string getNodeName() const { return "Node"; }

  virtual std::string asString(const std::string &prefix) const;
  virtual NodePtr getNextSibling() const;

  inline void setLocation(const CodeReference &location) {
    this->location = location;
  }
  inline CodeReference &getLocation() { return location; }

protected:
  CodeReference location;
};

}
}

#endif