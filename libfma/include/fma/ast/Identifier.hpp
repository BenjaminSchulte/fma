#ifndef __FMA_IDENTIFIER_H__
#define __FMA_IDENTIFIER_H__

#include "Node.hpp"

namespace FMA {
namespace ast {

class Expression;

/**
 * Expression
 */
class Identifier : public Node {
public:
  Identifier(const std::string &name);

  virtual std::string getNodeName() const { return "Identifier"; }
  virtual std::string asString(const std::string &prefix) const;
  void linkLastStatement(Identifier *next);

  virtual interpret::ResultPtr execute(const interpret::ContextPtr &context) const;
  Expression *createStringExpression() const;

  const std::string &getName() const { return name; }
  const IdentifierPtr &getNextIdentifier() const { return nextIdentifier; }

  void setRoot(bool newRoot) { root = newRoot; }
  inline bool isRoot() const { return root; }

protected:
  std::string name;
  bool root;
  IdentifierPtr nextIdentifier;
};

}
}

#endif