#include <json/JsonClass.hpp>
#include <json/JsonSax.hpp>
#include <fma/core/Number.hpp>
#include <fma/core/String.hpp>
#include <fma/core/Array.hpp>
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
using namespace FMA::json;
using namespace FMA::core;

// ----------------------------------------------------------------------------
ClassPtr JsonClass::create(const ModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("JSON", "fmaJSON::JSON"));
  klass->extends(ClassObject);

  klass->setMember("from_file", TypePtr(new InternalFunctionValue("from_file", JsonClass::from_file)));

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", JsonClass::initialize)));

  root->setMember("JSON", klass);
  return klass;
}

// ----------------------------------------------------------------------------
ResultPtr JsonClass::initialize(const ContextPtr &context, const GroupedParameterList &) {
  context->log().error() << "Can not instanciate class 'JSON'";

  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
ResultPtr JsonClass::from_file(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();

  if (!args.size()) {
    context->log().error() << "Missing filename";
    return ResultPtr(new Result());
  }

  std::string filename = args.front()->convertToString(context);
  std::ifstream is(filename);
  JsonSax sax(context);

  if (!nlohmann::json::sax_parse(is, &sax)) {
    context->log().error() << "Unable to parse JSON file: " << filename;
    return ResultPtr(new Result());
  }

  TypePtr result(sax.getRoot());

  return ResultPtr(new Result(context, result));
}
