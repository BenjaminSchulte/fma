#ifndef __FMA_EXPRESSION_H__
#define __FMA_EXPRESSION_H__

#include <vector>
#include "Node.hpp"

namespace FMA {
namespace ast {

/**
 * Expression
 */
class Expression : public Node {
public:
  virtual std::string getNodeName() const { return "Expression"; }
};

class MacroParameter : public Expression {
public:
  enum Type {
    DEFAULT,
    ARGS,
    KWARGS,
    BLOCK
  };

  MacroParameter(const IdentifierPtr &identifier) : type(DEFAULT), identifier(identifier), defaultValue() {}
  MacroParameter(const IdentifierPtr &identifier, const ExpressionPtr &defaultValue) : type(DEFAULT), identifier(identifier), defaultValue(defaultValue) {}

  inline void setType(Type newType) { type = newType; }

  inline bool isArgs() const { return type == ARGS; }
  inline bool isKwArgs() const { return type == KWARGS; }
  inline bool isBlock() const { return type == BLOCK; }

  inline bool hasDefaultValue() const { return !!defaultValue; }
  inline const ExpressionPtr &getDefaultValue() const { return defaultValue; }

  inline const IdentifierPtr &getIdentifier() const { return identifier; }

protected:
  Type type;
  IdentifierPtr identifier;
  ExpressionPtr defaultValue;
};

typedef std::shared_ptr<MacroParameter> MacroParameterPtr;
typedef std::vector<MacroParameterPtr> MacroParameterList;

class Parameter : public Expression {
public:
  enum Type {
    ARG,
    BLOCK,
    CONTEXT,
    KWARG,
    ARGS,
    KWARGS
  };

  Parameter() : type(ARG), key(""), value(NULL) {}
  Parameter(Type type, const ExpressionPtr &ptr) : type(type), key(""), value(ptr) {}
  Parameter(const ExpressionPtr &ptr) : type(ARG), key(""), value(ptr) {}
  Parameter(const std::string &key, const ExpressionPtr &ptr) : type(KWARG), key(key), value(ptr) {}

  virtual std::string getNodeName() const { return "Parameter"; }
  virtual interpret::ResultPtr execute(const interpret::ContextPtr &context) const;

  inline Type getType() const { return type; }
  inline const std::string &getKey() const { return key; }

  std::string asString(const std::string &prefix) const;

protected:
  Type type;
  std::string key;
  ExpressionPtr value;
};

typedef std::shared_ptr<Parameter> ParameterPtr;
typedef std::vector<ParameterPtr> ParameterList;

class InlineMacroExpression : public Expression {
public:
  InlineMacroExpression(const MacroParameterList &parameters, Statement *children);

  virtual interpret::ResultPtr execute(const interpret::ContextPtr &context) const;
  virtual std::string getNodeName() const { return "MacroDeclarationStatement"; }
  std::string asString(const std::string &prefix) const;

protected:
  StatementPtr children;
  MacroParameterList parameters;
};


class ConstantExpression : public Expression {
public:
  ConstantExpression(Expression *expression);

  virtual std::string getNodeName() const { return "ConstantExpression"; }

  std::string asString(const std::string &prefix) const;
  virtual interpret::ResultPtr execute(const interpret::ContextPtr &context) const;

protected:
  ExpressionPtr expression;
};

class ArrayExpression : public Expression {
public:
  ArrayExpression(const ParameterList &items);

  virtual std::string getNodeName() const { return "ArrayExpression"; }
  std::string asString(const std::string &prefix) const;
  virtual interpret::ResultPtr execute(const interpret::ContextPtr &context) const;

protected:
  ParameterList items;
};

class MapExpression : public Expression {
public:
  MapExpression(const ParameterList &items);

  virtual std::string getNodeName() const { return "MapExpression"; }
  std::string asString(const std::string &prefix) const;
  virtual interpret::ResultPtr execute(const interpret::ContextPtr &context) const;

protected:
  ParameterList items;
};

class UnaryOperationExpression : public Expression {
public:
  enum Operator {
    NOT,
    TILDE,
    MINUS,
    POINTER,
    PLUS
  };

  UnaryOperationExpression(Operator op, Expression *expression);

  virtual interpret::ResultPtr execute(const interpret::ContextPtr &context) const;
  virtual std::string getNodeName() const { return "UnaryOperationExpression"; }

  std::string asString(const std::string &prefix) const;

protected:
  Operator op;
  ExpressionPtr expression;
};

class IdentifierExpression : public Expression {
public:
  IdentifierExpression(Identifier *identifier);
  
  virtual interpret::ResultPtr execute(const interpret::ContextPtr &context) const;
  virtual std::string getNodeName() const { return "IdentifierExpression"; }

  std::string asString(const std::string &prefix) const;

protected:
  IdentifierPtr identifier;
};

class AssignExpression : public Expression {
public:
  AssignExpression(Expression *left, Expression *right);

  virtual interpret::ResultPtr execute(const interpret::ContextPtr &context) const;
  virtual std::string getNodeName() const { return "AssignExpression"; }

  std::string asString(const std::string &prefix) const;

protected:
  ExpressionPtr left;
  ExpressionPtr right;
};


class OperationExpression : public Expression {
public:
  enum Operator {
    LOG_AND,
    LOG_OR,
    ADD,
    SUB,
    MUL,
    DIV,
    REM,
    AND,
    OR,
    XOR,
    LSHIFT,
    RSHIFT,
    ASSIGN_LOG_AND,
    ASSIGN_LOG_OR,
    ASSIGN_ADD,
    ASSIGN_SUB,
    ASSIGN_MUL,
    ASSIGN_DIV,
    ASSIGN_REM,
    ASSIGN_AND,
    ASSIGN_OR,
    ASSIGN_XOR,
    ASSIGN_LSHIFT,
    ASSIGN_RSHIFT,
    EQUALITY,
    NOT_EQUALITY,
    LESSTHAN_EQUAL,
    GREATERTHAN_EQUAL,
    LESSTHAN,
    GREATERTHAN,
    INDEX,
    MEMBER
  };

  OperationExpression(Expression *left, Operator op, Expression *right);

  virtual interpret::ResultPtr calculate(const interpret::ContextPtr &context, const interpret::ResultPtr &left, const interpret::ResultPtr &right, const char *op) const;
  virtual interpret::ResultPtr execute(const interpret::ContextPtr &context) const;
  virtual std::string getNodeName() const { return "OperationExpression"; }

  std::string asString(const std::string &prefix) const;

  static bool isAssign(Operator op);

protected:
  ExpressionPtr left;
  Operator op;
  ExpressionPtr right;
};

/**
 * Call expression
 */
class CallExpression : public Expression {
public:
  CallExpression(Expression *expression);

  std::string asString(const std::string &prefix) const;
  void setArguments(const ParameterList &args) {
    this->arguments = args;
  }

  virtual interpret::ResultPtr execute(const interpret::ContextPtr &context) const;
  virtual std::string getNodeName() const { return "CallExpression"; }

protected:
  ExpressionPtr expression;
  ParameterList arguments;
};

/**
 * Unary expression
 */
class UnaryExpression : public Expression {
public:

  virtual std::string getNodeName() const { return "UnaryExpression"; }
};

/**
 * Constant number
 */
class ConstantNumber : public UnaryExpression {
private:
  int64_t number;

public:
  ConstantNumber(const int64_t number);

  virtual std::string getNodeName() const { return "ConstantNumber"; }
  virtual interpret::ResultPtr execute(const interpret::ContextPtr &context) const;
  std::string asString(const std::string &prefix) const;
};

/**
 * Constant number
 */
class ConstantRange : public UnaryExpression {
private:
  ExpressionPtr first;
  ExpressionPtr last;

public:
  ConstantRange(Expression *first, Expression *last);

  virtual std::string getNodeName() const { return "ConstantRange"; }
  virtual interpret::ResultPtr execute(const interpret::ContextPtr &context) const;
  std::string asString(const std::string &prefix) const;
};

/**
 * Constant number
 */
class ConstantBoolean : public UnaryExpression {
private:
  uint8_t number;

public:
  ConstantBoolean(const uint8_t number);

  virtual std::string getNodeName() const { return "ConstantBoolean"; }
  virtual interpret::ResultPtr execute(const interpret::ContextPtr &context) const;
  std::string asString(const std::string &prefix) const;
};

/**
 * Constant number
 */
class NilExpression : public UnaryExpression {
public:
  NilExpression();

  virtual std::string getNodeName() const { return "NilExpression"; }
  virtual interpret::ResultPtr execute(const interpret::ContextPtr &context) const;
  std::string asString(const std::string &prefix) const;
};

/**
 * String
 */
class StringLiteral : public UnaryExpression {
public:
  std::string string;

public:
  StringLiteral(const std::string &string);

  virtual std::string getNodeName() const { return "StringLiteral"; }
  virtual interpret::ResultPtr execute(const interpret::ContextPtr &context) const;
  std::string asString(const std::string &prefix) const;
};

}
}

#endif