#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <fs/DirClass.hpp>
#include <fma/core/Boolean.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/types/Undefined.hpp>
#include <fma/types/Object.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/Project.hpp>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace FMA;
using namespace FMA::types;
using namespace FMA::interpret;
using namespace FMA::fs;
using namespace FMA::core;

// ----------------------------------------------------------------------------
ClassPtr DirClass::create(const ModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("Dir", "fs::Dir"));
  klass->extends(ClassObject);

  klass->setMember("mkdir", TypePtr(new InternalFunctionValue("mkdir", DirClass::mkdir)));
  klass->setMember("mkdir_p", TypePtr(new InternalFunctionValue("mkdir_p", DirClass::mkdir_p)));

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", DirClass::initialize)));

  root->setMember("Dir", klass);
  return klass;
}

// ----------------------------------------------------------------------------
ResultPtr DirClass::initialize(const ContextPtr &context, const GroupedParameterList &) {
  context->log().error() << "Can not instanciate class 'Dir'";

  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
ResultPtr DirClass::mkdir(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    return ResultPtr(new Result());
  }

  boost::filesystem::create_directory(args.front()->convertToString(context));
  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
ResultPtr DirClass::mkdir_p(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    return ResultPtr(new Result());
  }

  boost::filesystem::create_directories(args.front()->convertToString(context));
  return ResultPtr(new Result());
}
