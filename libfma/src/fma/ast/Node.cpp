#include <fma/ast/Node.hpp>
#include <fma/Project.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Result.hpp>

using namespace FMA::ast;
using namespace FMA::interpret;

// ---------------------------------------------------------------------------
NodePtr Node::getNextSibling() const {
  return NodePtr(NULL);
}

// ---------------------------------------------------------------------------
std::string Node::asString(const std::string &) const {
  return "???";
}

// ---------------------------------------------------------------------------
ResultPtr Node::execute(const ContextPtr &context) const {
  context->log().error() << location << "Unhandled node type in interpreter: " << getNodeName();

  return ResultPtr();
}

// ----------------------------------------------------------------------------
