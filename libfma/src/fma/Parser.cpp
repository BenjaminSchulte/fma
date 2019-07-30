#include <stdio.h>
#include <fma/Parser.hpp>
#include <fma/Project.hpp>
#include <fma/File.hpp>
#include <fma/FileMap.hpp>
#include <fma/ast/Node.hpp>

using namespace FMA;

typedef struct yy_buffer_state * YY_BUFFER_STATE;
typedef void *yyscan_t;
int yylex_init(yyscan_t*);
int yylex_destroy(yyscan_t);
int yyparse(yyscan_t);
void yyset_in(FILE *_in_str, yyscan_t yyscanner);
YY_BUFFER_STATE yy_scan_string (const char *yy_str, yyscan_t yyscanner);
void yy_delete_buffer(YY_BUFFER_STATE buffer, yyscan_t yyscanner);

ast::Node *parsedRootNode;
Parser *yyCurrentParser;

//extern int yydebug;

// ----------------------------------------------------------------------------
Parser::Parser(Project *project)
  : project(project)
{
  //yydebug = 1;
}

// ----------------------------------------------------------------------------
Parser::~Parser() {
}

// ----------------------------------------------------------------------------
bool Parser::parseString(const char *string) {
  yyCurrentParser = this;
  currentFile = NULL;
  parsedRootNode = NULL;

  yyscan_t scanner;
  yylex_init(&scanner);
  YY_BUFFER_STATE buffer = yy_scan_string(string, scanner);
  yyparse(scanner);
  yy_delete_buffer(buffer, scanner);
  yylex_destroy(scanner);

  result = ast::NodePtr(parsedRootNode);

  return errors.size() == 0;
}

// ----------------------------------------------------------------------------
bool Parser::parseFile(const char *fileName, const FilePtr &relativeTo) {
  project->log().trace() << "Trying to locate file " << fileName;

  FileMap *map = project->getFiles();
  FilePtr file = map->resolve(fileName, relativeTo);
  if (!file) {
    return false;
  }

  FILE *fh = fopen(file->absoluteFileName().c_str(), "rb");
  if (fh == NULL) {
    return false;
  }

  yyscan_t scanner;
  yylex_init(&scanner);
  yyset_in(fh, scanner);
  yyCurrentParser = this;
  currentFile = file;
  // yylineno = 1;
  yyparse(scanner);
  // yyin = NULL;
  currentFile = NULL;
  yylex_destroy(scanner);

  if (errors.size() > 0) {
    return false;
  }

  result = ast::NodePtr(parsedRootNode);
  file->setRootNode(result);

  return true;
}

// ----------------------------------------------------------------------------
const ast::NodePtr &Parser::getResult() const {
  return result;
}

// ----------------------------------------------------------------------------
CodeReference Parser::reference(unsigned int line, unsigned int column) const {
  return CodeReference(currentFile, line, column);
}

// ----------------------------------------------------------------------------
void Parser::error(unsigned int line, unsigned int column, const std::string &message) {
  errors.push_back(Error(Error::ERROR, reference(line, column), message));
}

// ----------------------------------------------------------------------------
