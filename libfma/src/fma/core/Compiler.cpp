#include <boost/filesystem.hpp>
#include <fma/core/Class.hpp>
#include <fma/core/Compiler.hpp>
#include <fma/core/String.hpp>
#include <fma/core/SymbolReference.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/Parameter.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/plugin/MemoryPluginAdapter.hpp>
#include <fma/Project.hpp>

using namespace FMA;
using namespace FMA::core;
using namespace FMA::types;
using namespace FMA::interpret;

#include <iostream>
#include <sstream>

// ----------------------------------------------------------------------------
ClassPtr CompilerClass::create(const RootModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("Compiler", "Compiler"));
  klass->extends(ClassObject);

  klass->setMember("PROJECT_DIR", TypePtr(new InternalFunctionValue("PROJECT_DIR", CompilerClass::PROJECT_DIR)));

  klass->setMember("assert", TypePtr(new InternalFunctionValue("assert", CompilerClass::_assert)));
  klass->setMember("dump", TypePtr(new InternalFunctionValue("dump", CompilerClass::dump)));
  klass->setMember("print", TypePtr(new InternalFunctionValue("print", CompilerClass::print)));
  klass->setMember("command", TypePtr(new InternalFunctionValue("command", CompilerClass::command)));

  klass->setMember("trace", TypePtr(new InternalFunctionValue("trace", CompilerClass::trace)));
  klass->setMember("debug", TypePtr(new InternalFunctionValue("debug", CompilerClass::debug)));
  klass->setMember("info", TypePtr(new InternalFunctionValue("info", CompilerClass::info)));
  klass->setMember("notice", TypePtr(new InternalFunctionValue("notice", CompilerClass::notice)));
  klass->setMember("warn", TypePtr(new InternalFunctionValue("warn", CompilerClass::warn)));
  klass->setMember("error", TypePtr(new InternalFunctionValue("error", CompilerClass::error)));
  klass->setMember("fatal", TypePtr(new InternalFunctionValue("fatal", CompilerClass::fatal)));

  root->setMember("Compiler", klass);
  return klass;
}

// ----------------------------------------------------------------------------
ResultPtr CompilerClass::PROJECT_DIR(const ContextPtr &context, const GroupedParameterList &) {
  return StringClass::createInstance(context, boost::filesystem::current_path().string());
}

// ----------------------------------------------------------------------------
ResultPtr CompilerClass::trace(const ContextPtr &context, const GroupedParameterList &parameter) {
  return _log(LogLevel::FMA_LOG_TRACE, context, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr CompilerClass::debug(const ContextPtr &context, const GroupedParameterList &parameter) {
  return _log(LogLevel::FMA_LOG_DEBUG, context, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr CompilerClass::info(const ContextPtr &context, const GroupedParameterList &parameter) {
  return _log(LogLevel::FMA_LOG_INFO, context, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr CompilerClass::notice(const ContextPtr &context, const GroupedParameterList &parameter) {
  return _log(LogLevel::FMA_LOG_NOTICE, context, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr CompilerClass::warn(const ContextPtr &context, const GroupedParameterList &parameter) {
  return _log(LogLevel::FMA_LOG_WARN, context, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr CompilerClass::error(const ContextPtr &context, const GroupedParameterList &parameter) {
  return _log(LogLevel::FMA_LOG_ERROR, context, parameter);
}

// ----------------------------------------------------------------------------
ResultPtr CompilerClass::fatal(const ContextPtr &context, const GroupedParameterList &parameter) {
  return _log(LogLevel::FMA_LOG_FATAL, context, parameter);
}

// ----------------------------------------------------------------------------
std::string CompilerClass::_msg(const ContextPtr &context, const GroupedParameterList &parameter) {
  std::stringstream os;
  bool first = true;

  for (auto &param : parameter.only_args()) {
    auto value = param;

    if (first) {
      first = false;
    } else {
      os << " ";
    }

    if (value->hasMember("to_s")) {
      os << param->convertToString(context);
    } else {
      os << value->asString();
    }
  }

  return os.str();
}

// ----------------------------------------------------------------------------
ResultPtr CompilerClass::_log(LogLevel level, const ContextPtr &context, const GroupedParameterList &parameter) {
  context->getProject()->log().write(level, _msg(context, parameter));
  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
ResultPtr CompilerClass::print(const ContextPtr &context, const GroupedParameterList &parameter) {
  std::cout << _msg(context, parameter) << std::endl;

  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
ResultPtr CompilerClass::command(const ContextPtr &context, const GroupedParameterList &parameter) {
  std::stringstream os;

  for (auto &param : parameter.only_args()) {
    auto value = param;

    if (value->hasMember("to_s")) {
      os << param->convertToString(context);
    } else {
      os << value->asString();
    }
  }

  return SymbolReferenceClass::createInstance(context, context->getProject()->getMemoryAdapter()->getSymbolMap()->createCommand(os.str()));
}

// ----------------------------------------------------------------------------
ResultPtr CompilerClass::dump(const ContextPtr &, const GroupedParameterList &parameter) {
  for (auto &param : parameter.only_args()) {
    param->dump("");
  }

  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
ResultPtr CompilerClass::_assert(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    return ResultPtr(new Result());
  }

  const TypePtr &test = args.front();
  if (!test->convertToBoolean(context)) {
    context->log().error() << "Assertion failed.";
  }

  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------