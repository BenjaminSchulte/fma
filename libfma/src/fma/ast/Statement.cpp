#include <sstream>
#include <fma/ast/Expression.hpp>
#include <fma/ast/Statement.hpp>
#include <fma/ast/Identifier.hpp>

using namespace FMA::ast;

// ----------------------------------------------------------------------------
Statement::Statement()
  : nextStatement(NULL)
{
}

// ----------------------------------------------------------------------------
Statement::~Statement() {
}

// ----------------------------------------------------------------------------
NodePtr Statement::getNextSibling() const {
  return nextStatement;
}

// ----------------------------------------------------------------------------
void Statement::linkLastStatement(Statement *next) {
  Statement* current = this;

  while (current->nextStatement) {
    current = current->nextStatement.get();
  }

  current->nextStatement = StatementPtr(next);
}
