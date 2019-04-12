#include <fma/core/Class.hpp>
#include <fma/core/Function.hpp>
#include <fma/core/String.hpp>
#include <fma/core/DataBlock.hpp>
#include <fma/core/Number.hpp>
#include <fma/core/Boolean.hpp>
#include <fma/core/SymbolReference.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/types/Object.hpp>
#include <fma/types/Macro.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/Parameter.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/plugin/MemoryPluginAdapter.hpp>
#include <fma/symbol/SymbolReference.hpp>
#include <fma/serialize/SerializeTypes.hpp>
#include <fma/Project.hpp>

using namespace FMA;
using namespace FMA::core;
using namespace FMA::types;
using namespace FMA::interpret;
using namespace FMA::symbol;
using namespace FMA::plugin;

#include <iostream>
#include <sstream>

// ----------------------------------------------------------------------------
ClassPtr FunctionClass::create(const RootModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("Function", "Function"));
  klass->extends(ClassObject);

  klass->setMember("@FMA_TYPE_ID", TypePtr(new InternalNumberValue(serialize::TYPE_FUNCTION)));

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", FunctionClass::initialize)));

  root->setMember("Function", klass);
  return klass;
}

// ----------------------------------------------------------------------------
ResultPtr FunctionClass::createInstance(const ContextPtr &context, const MacroPtr &macro) {
  // Resolve a 
  ClassPtr number = context->getRootLevelContext()->resolve("Function")->asClass();
  if (!number) {
    return ResultPtr(new Result());
  }

  GroupedParameterList parameters;
  parameters.push_back(StringClass::createInstance(context, context->getNameHint())->get());
  parameters.push_back(Parameter(Parameter::BLOCK, std::dynamic_pointer_cast<Base>(macro)));
  return Result::executed(context, number->createInstance(context, parameters));
}

// ----------------------------------------------------------------------------
ResultPtr FunctionClass::initialize(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  const TypeList &blocks = parameter.only_blocks();
  TypePtr name;
  
  if (args.size()) {
    name = args.front();
  }

  if (blocks.size()) {
    TypePtr macro = blocks.front();
    context->self()->setMember("#macro", macro);
  }

  MemoryBlock *block = DataBlockClass::memoryBlock(context);
  if (block && !name->isUndefined()) {
    block->setNameHint(name->convertToString(context));
  }

  return Result::executed(context, context->self());
}