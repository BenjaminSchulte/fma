#include <sstream>
#include <fma/ast/Expression.hpp>
#include <fma/ast/Statement.hpp>
#include <fma/ast/Identifier.hpp>
#include <fma/types/Base.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/Project.hpp>
#include <fma/Parser.hpp>
#include <fma/File.hpp>

using namespace FMA::ast;
using namespace FMA::interpret;

// ----------------------------------------------------------------------------
RequireStatement::RequireStatement(Expression *expression)
  : expression(expression)
{
}

// ----------------------------------------------------------------------------
std::string RequireStatement::asString(const std::string &prefix) const {
  std::ostringstream os;
  os << prefix << "require " << expression->asString("") << std::endl;
  if (nextStatement) { os << nextStatement->asString(prefix); }
  return os.str();
}

// ----------------------------------------------------------------------------
ResultPtr RequireStatement::execute(const ContextPtr &context) const {
  ResultPtr fileNameObj = context->execute(expression);
  std::string fileName = fileNameObj->get()->convertToString(context);

  if (!fileName.length()) {
    context->log().error() << location << "Missing filename for require statement";
    return ResultPtr(new Result());
  }

  Parser parser(context->getProject());
  FilePtr file = location.file.lock();
  if (!parser.parseFile(fileName.c_str(), file)) {
    context->log().error() << location << "Failed to parse input file: " << fileName;
    for(auto const& error: parser.getErrors()) {
      context->log().error() << location << error.file.line << ":" << error.file.col << ": " << error.message;
    }

    return ResultPtr(new Result());
  }
  
  return context->execute(parser.getResult());
}