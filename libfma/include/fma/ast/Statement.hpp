#ifndef __FMA_STATEMENT_H__
#define __FMA_STATEMENT_H__

#include "Node.hpp"
#include "Expression.hpp"
#include <vector>

namespace FMA {
namespace types {
  class Decoratable;
}
namespace ast {

typedef std::shared_ptr<class Parameter> ParameterPtr;
typedef std::vector<ParameterPtr> ParameterList;

/**
 * Expression
 */
class Statement : public Node {
public:
  Statement();
  virtual ~Statement();

  void linkLastStatement(Statement *next);
  virtual std::string getNodeName() const { return "Statement"; }
  virtual NodePtr getNextSibling() const;

protected:
  StatementPtr nextStatement;
};

class ModuleDeclarationStatement : public Statement {
public:
  ModuleDeclarationStatement(class Identifier *identifier, Statement *children);
  virtual ~ModuleDeclarationStatement();

  virtual interpret::ResultPtr execute(const interpret::ContextPtr &context) const;
  virtual std::string getNodeName() const { return "ModuleDeclarationStatement"; }
  std::string asString(const std::string &prefix) const;

protected:
  IdentifierPtr identifier;
  StatementPtr children;
};

class ClassDeclarationStatement : public Statement {
public:
  ClassDeclarationStatement(class Identifier *identifier, class Identifier *parent, Statement *children);
  virtual ~ClassDeclarationStatement();

  virtual interpret::ResultPtr execute(const interpret::ContextPtr &context) const;
  virtual std::string getNodeName() const { return "ClassDeclarationStatement"; }
  std::string asString(const std::string &prefix) const;

protected:
  IdentifierPtr identifier;
  IdentifierPtr parent;
  StatementPtr children;
};

class LabelStatement : public Statement {
public:
  LabelStatement(class Identifier *identifier);
  virtual ~LabelStatement();

  virtual interpret::ResultPtr execute(const interpret::ContextPtr &context) const;
  virtual std::string getNodeName() const { return "LabelStatement"; }
  std::string asString(const std::string &prefix) const;

  void registerLabel(const interpret::ContextPtr &context, const std::string &name);
  static void registerLabels(const interpret::ContextPtr &context, const std::string &name, const StatementPtr &statement);

protected:
  IdentifierPtr identifier;
};

class ImportStatement : public Statement {
public:
  ImportStatement(class Identifier *identifier);
  virtual ~ImportStatement();

  virtual interpret::ResultPtr execute(const interpret::ContextPtr &context) const;
  virtual std::string getNodeName() const { return "ImportStatement"; }
  std::string asString(const std::string &prefix) const;

protected:
  IdentifierPtr identifier;
};

class ExternStatement : public Statement {
public:
  ExternStatement(class Identifier *identifier);
  virtual ~ExternStatement();

  virtual interpret::ResultPtr execute(const interpret::ContextPtr &context) const;
  virtual std::string getNodeName() const { return "ExternStatement"; }
  std::string asString(const std::string &prefix) const;

protected:
  IdentifierPtr identifier;
};

class DecoratableStatement : public Statement {
public:
  DecoratableStatement() : decorator(NULL) {};

  inline void setDecoratorStatement(const StatementPtr &decorator) {
    this->decorator = decorator;
  };

  void attachDecoratable(const interpret::ContextPtr &context, types::Decoratable *object) const;
  interpret::ResultPtr wrapDecoratable(const interpret::ContextPtr &context, const interpret::ResultPtr &result) const;

protected:
  StatementPtr decorator;
};

class DecoratorCallStatement : public DecoratableStatement {
public:
  DecoratorCallStatement(Identifier *identifier, const ParameterList &parameters);

  virtual std::string getNodeName() const { return "DecoratorCallStatement"; }
  virtual interpret::ResultPtr execute(const interpret::ContextPtr &context) const;

protected:
  IdentifierPtr identifier;
  ParameterList parameters;
};

class MacroDeclarationStatement : public DecoratableStatement {
public:
  MacroDeclarationStatement(class Identifier *identifier, const MacroParameterList &parameters, Statement *children, bool isStatic=false);

  virtual interpret::ResultPtr execute(const interpret::ContextPtr &context) const;
  virtual std::string getNodeName() const { return "MacroDeclarationStatement"; }
  std::string asString(const std::string &prefix) const;
  inline void setIsDecorator() { isDecorator = true; }

protected:
  IdentifierPtr identifier;
  StatementPtr children;
  MacroParameterList parameters;
  bool isStatic;
  bool isDecorator;
};

class DataBlockDeclarationStatement : public DecoratableStatement {
public:
  DataBlockDeclarationStatement(class Identifier *identifier, Statement *children);

  virtual interpret::ResultPtr execute(const interpret::ContextPtr &context) const;
  virtual std::string getNodeName() const { return "DataBlockDeclarationStatement"; }
  std::string asString(const std::string &prefix) const;

protected:
  IdentifierPtr identifier;
  StatementPtr children;
};

class FunctionDeclarationStatement : public DecoratableStatement {
public:
  FunctionDeclarationStatement(class Identifier *identifier, Statement *children, bool exported=false);

  virtual interpret::ResultPtr execute(const interpret::ContextPtr &context) const;
  virtual std::string getNodeName() const { return "FunctionDeclarationStatement"; }
  std::string asString(const std::string &prefix) const;

protected:
  IdentifierPtr identifier;
  StatementPtr children;
  bool exported;
};

class ExpressionStatement : public Statement {
private:
  ExpressionPtr expression;

public:
  ExpressionStatement(Expression *expression);
  virtual ~ExpressionStatement();

  virtual interpret::ResultPtr execute(const interpret::ContextPtr &context) const;
  virtual std::string getNodeName() const { return "ExpressionStatement"; }
  std::string asString(const std::string &prefix) const;
};

class ReturnStatement : public Statement {
private:
  ExpressionPtr expression;

public:
  ReturnStatement();
  ReturnStatement(Expression *expression);
  virtual ~ReturnStatement();

  virtual interpret::ResultPtr execute(const interpret::ContextPtr &context) const;
  virtual std::string getNodeName() const { return "ReturnStatement"; }
  std::string asString(const std::string &prefix) const;
};

class RaiseStatement : public Statement {
private:
  ExpressionPtr expression;

public:
  RaiseStatement();
  RaiseStatement(Expression *expression);
  virtual ~RaiseStatement();

  virtual interpret::ResultPtr execute(const interpret::ContextPtr &context) const;
  virtual std::string getNodeName() const { return "RaiseStatement"; }
  std::string asString(const std::string &prefix) const;
};

class RequireStatement : public Statement {
private:
  ExpressionPtr expression;

public:
  RequireStatement(Expression *expression);

  virtual std::string getNodeName() const { return "RequireStatement"; }
  virtual interpret::ResultPtr execute(const interpret::ContextPtr &context) const;
  std::string asString(const std::string &prefix) const;
};

class IfStatement : public Statement {
private:
  ExpressionPtr expression;
  StatementPtr thenBranch;
  StatementPtr elseBranch;

public:
  IfStatement(Expression *expression);

  inline void setThen(Statement *statement) { thenBranch = StatementPtr(statement); }
  inline void setElse(Statement *statement) { elseBranch = StatementPtr(statement); }

  virtual interpret::ResultPtr execute(const interpret::ContextPtr &context) const;

  virtual std::string getNodeName() const { return "IfStatement"; }
  std::string asString(const std::string &prefix) const;
};

struct CaseStatementCase {
  CaseStatementCase() : expression(), statement() {};
  CaseStatementCase(class Statement *stmt) : expression(), statement(stmt) {}
  CaseStatementCase(class Expression *expr, class Statement *stmt) : expression(expr), statement(stmt) {}

  ExpressionPtr expression;
  StatementPtr statement;
};

typedef std::vector<CaseStatementCase> CaseStatementCaseList;

class CaseStatement : public Statement {
private:
  ExpressionPtr expression;
  CaseStatementCaseList cases;

public:
  CaseStatement(Expression *expression, const CaseStatementCaseList &cases);

  virtual interpret::ResultPtr execute(const interpret::ContextPtr &context) const;

  virtual std::string getNodeName() const { return "CaseStatement"; }
  std::string asString(const std::string &prefix) const;
};

}
}

#endif