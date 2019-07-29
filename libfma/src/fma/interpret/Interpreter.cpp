#include <fma/interpret/Interpreter.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/ProjectContext.hpp>
#include <fma/interpret/InterpreterQueue.hpp>
#include <fma/ast/Node.hpp>
#include <fma/Project.hpp>

using namespace FMA;
using namespace FMA::interpret;

// ----------------------------------------------------------------------------
Interpreter::Interpreter(Project *project)
  : project(project)
  , queue(new InterpreterQueue(this))
{
}

// ----------------------------------------------------------------------------
Interpreter::~Interpreter() {
  delete queue;
}

// ----------------------------------------------------------------------------
ResultPtr Interpreter::execute(const ast::NodePtr &node) {
  ResultPtr result = executeWithoutPostProcess(node);
  postProcess();
  return result;
}

// ----------------------------------------------------------------------------
ResultPtr Interpreter::executeWithoutPostProcess(const ast::NodePtr &node) {
  std::shared_ptr<ProjectContext> context(new ProjectContext(this));
  
  return context->execute(node);
}

// ----------------------------------------------------------------------------
void Interpreter::postProcess() {
  project->log().debug() << "Processing queued tasks.";
  while (queue->more()) {
    queue->execute();
  }
}

// ----------------------------------------------------------------------------
