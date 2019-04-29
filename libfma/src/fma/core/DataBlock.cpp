#include <fma/core/Class.hpp>
#include <fma/core/DataBlock.hpp>
#include <fma/core/String.hpp>
#include <fma/core/Number.hpp>
#include <fma/core/Buffer.hpp>
#include <fma/core/Boolean.hpp>
#include <fma/core/SymbolReference.hpp>
#include <fma/output/DynamicBuffer.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/types/Object.hpp>
#include <fma/types/Undefined.hpp>
#include <fma/types/Macro.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/types/DecoratorContainer.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Interpreter.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/InstanceContext.hpp>
#include <fma/interpret/Parameter.hpp>
#include <fma/interpret/InterpreterQueue.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/symbol/SymbolReference.hpp>
#include <fma/plugin/MemoryPluginAdapter.hpp>
#include <fma/serialize/SerializeTypes.hpp>
#include <fma/Project.hpp>

using namespace FMA;
using namespace FMA::core;
using namespace FMA::types;
using namespace FMA::interpret;
using namespace FMA::plugin;
using namespace FMA::output;
using namespace FMA::symbol;

#include <iostream>
#include <sstream>

// ----------------------------------------------------------------------------
ClassPtr DataBlockClass::create(const RootModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("DataBlock", "DataBlock"));
  klass->extends(ClassObject);

  klass->setMember("@FMA_TYPE_ID", TypePtr(new InternalNumberValue(serialize::TYPE_DATABLOCK)));

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", DataBlockClass::initialize)));
  proto->setMember("()", TypePtr(new InternalFunctionValue("()", DataBlockClass::op_call)));

  proto->setMember("override_located_at", TypePtr(new InternalFunctionValue("override_located_at", DataBlockClass::override_located_at)));
  proto->setMember("allow_located_at", TypePtr(new InternalFunctionValue("allow_located_at", DataBlockClass::allow)));
  proto->setMember("deny_located_at", TypePtr(new InternalFunctionValue("deny_located_at", DataBlockClass::deny)));

  proto->setMember("db", TypePtr(new InternalFunctionValue("db", DataBlockClass::db)));
  proto->setMember("dw", TypePtr(new InternalFunctionValue("dw", DataBlockClass::dw)));
  proto->setMember("dd", TypePtr(new InternalFunctionValue("dd", DataBlockClass::dd)));
  proto->setMember("create_label", TypePtr(new InternalFunctionValue("create_label", DataBlockClass::create_label)));
  proto->setMember("label", TypePtr(new InternalFunctionValue("label", DataBlockClass::label)));
  proto->setMember("to_n", TypePtr(new InternalFunctionValue("to_n", DataBlockClass::to_n)));
  proto->setMember("to_s", TypePtr(new InternalFunctionValue("to_s", DataBlockClass::to_s)));
  proto->setMember("to_sym", TypePtr(new InternalFunctionValue("to_sym", DataBlockClass::to_sym)));
  proto->setMember("to_b", TypePtr(new InternalFunctionValue("to_b", DataBlockClass::to_b)));
  proto->setMember("__current_block", TypePtr(new InternalFunctionValue("__current_block", DataBlockClass::__current_block)));

  proto->setMember("::db", proto->getMember("db"));
  proto->setMember("::dw", proto->getMember("dw"));
  proto->setMember("::dd", proto->getMember("dd"));
  proto->setMember("::label", proto->getMember("label"));
  proto->setMember("::__current_block", proto->getMember("__current_block"));

  root->setMember("DataBlock", klass);
  return klass;
}

// ----------------------------------------------------------------------------
ResultPtr DataBlockClass::to_b(const ContextPtr &context, const GroupedParameterList &) {
  return BooleanClass::createInstance(context, true);
}

// ----------------------------------------------------------------------------
ResultPtr DataBlockClass::to_s(const ContextPtr &context, const GroupedParameterList &) {
  MemoryBlock *block = memoryBlock(context);
  if (!block) {
    return StringClass::createInstance(context, "<Function>");
  }

  return StringClass::createInstance(context, block->getNameHint());
}

// ----------------------------------------------------------------------------
ResultPtr DataBlockClass::to_sym(const ContextPtr &context, const GroupedParameterList &) {
  MemoryBlock *block = memoryBlock(context);
  if (!block) {
    return ResultPtr(new Result());
  }

  SymbolReferencePtr symbol = block->createReference();
  return SymbolReferenceClass::createInstance(context, symbol);
}


// ----------------------------------------------------------------------------
ResultPtr DataBlockClass::createInstance(const ContextPtr &context, const MacroPtr &macro) {
  // Resolve a 
  ClassPtr number = context->getRootLevelContext()->resolve("DataBlock")->asClass();
  if (!number) {
    return ResultPtr(new Result());
  }

  GroupedParameterList parameters;
  parameters.push_back(StringClass::createInstance(context, context->getNameHint())->get());
  parameters.push_back(Parameter(Parameter::BLOCK, std::dynamic_pointer_cast<Base>(macro)));
  return Result::executed(context, number->createInstance(context, parameters));
}

// ----------------------------------------------------------------------------
ResultPtr DataBlockClass::initialize(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  const TypeList &blocks = parameter.only_blocks();
  TypePtr name(new Undefined());
  
  if (args.size()) {
    name = args.front();
  }

  if (blocks.size()) {
    context->self()->setMember("#macro", blocks.front());
  }

  MemoryBlock *block = memoryBlock(context);
  if (block && !name->isUndefined()) {
    block->setNameHint(name->convertToString(context));
  }

  if (args.size() == 2) {
    const TypePtr &second = args.at(1);

    if (second->isObjectOfType("Buffer")) {
      const DynamicBufferPtr &buf = BufferClass::getBuffer(context->getProject(), second);
      if (buf) {
        block->write(buf->getData(), buf->getSize());
      }
    }
  }

  return Result::executed(context, context->self());
}

// ----------------------------------------------------------------------------
ResultPtr DataBlockClass::override_located_at(const ContextPtr &context, const GroupedParameterList &parameter) {
  MemoryBlock *block = memoryBlock(context);
  if (!block) {
    return ResultPtr(new Result());
  }

  block->location()->overrideAllow(context, parameter);
  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
ResultPtr DataBlockClass::allow(const ContextPtr &context, const GroupedParameterList &parameter) {
  MemoryBlock *block = memoryBlock(context);
  if (!block) {
    return ResultPtr(new Result());
  }

  block->location()->allow(context, parameter);
  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
ResultPtr DataBlockClass::deny(const ContextPtr &context, const GroupedParameterList &parameter) {
  MemoryBlock *block = memoryBlock(context);
  if (!block) {
    return ResultPtr(new Result());
  }

  block->location()->deny(context, parameter);
  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
ResultPtr DataBlockClass::op_call(const ContextPtr &context, const GroupedParameterList &params) {
  MemoryBlock *block = memoryBlock(context);
  if (!block) {
    return Result::executed(context, context->self());
  }

  TypePtr macroPtr = context->self()->getMember("#macro");
  if (!macroPtr->isMacro()) {
    block->markIsUsed(true);
    return Result::executed(context, context->self());
  }

  MacroPtr macro = macroPtr->asMacro();
  if (!macro->hasDecorator()) {
    return op_call_direct(context, params);
  }

  DecoratablePtr function(new InternalFunctionValue(block->getNameHint(), DataBlockClass::op_call_direct));
  TypePtr selfCall(new DecoratorCall(function, context, params, DECORATORCALL_OUTER));

  GroupedParameterList callParams;
  callParams.push_back(interpret::Parameter(interpret::Parameter::BLOCK, selfCall));

  return macro->invokeDecoratorWithParams(context, callParams, DECORATORCALL_OUTER);
}

// ----------------------------------------------------------------------------
ResultPtr DataBlockClass::op_call_direct(const ContextPtr &context, const GroupedParameterList &) {
  MemoryBlock *block = memoryBlock(context);
  if (!block) {
    return Result::executed(context, context->self());
  }

  block->markIsUsed(true);

  TypePtr macro = context->self()->getMember("#macro");
  if (macro->isMacro()) {
    ContextPtr callContext(context);

    TypePtr container = context->self()->getMember(FMA_TYPE_DECORATORCONTAINER_MEMBER);
    if (container->isDecoratorContainer()) {
      std::dynamic_pointer_cast<DecoratorContainer>(container)->attachDecorators(macro->asMacro());
    }

    GroupedParameterList params;
    context->getInterpreter()->getQueue()->queue(callContext, macro->asMacro(), params, true);
    context->self()->setMember("#macro", TypePtr(new Undefined()));
  }
  
  return Result::executed(context, context->self());
}

// ----------------------------------------------------------------------------
ResultPtr DataBlockClass::__current_block(const ContextPtr &context, const GroupedParameterList &) {
  return Result::executed(context, context->self());
}

// ----------------------------------------------------------------------------
ResultPtr DataBlockClass::db(const ContextPtr &context, const GroupedParameterList &parameter) {
  return writeData(context, parameter, 1);
}

// ----------------------------------------------------------------------------
ResultPtr DataBlockClass::dw(const ContextPtr &context, const GroupedParameterList &parameter) {
  return writeData(context, parameter, 2);
}

// ----------------------------------------------------------------------------
ResultPtr DataBlockClass::dd(const ContextPtr &context, const GroupedParameterList &parameter) {
  return writeData(context, parameter, 4);
}

// ----------------------------------------------------------------------------
ResultPtr DataBlockClass::to_n(const ContextPtr &context, const GroupedParameterList &) {
  MemoryBlock *block = memoryBlock(context);
  if (!block) {
    return ResultPtr(new Result());
  }

  return SymbolReferenceClass::createInstance(context, block->createReference());
}

// ----------------------------------------------------------------------------
ResultPtr DataBlockClass::create_label(const ContextPtr &context, const GroupedParameterList &parameter) {
  MemoryBlock *block = memoryBlock(context);
  if (!block) {
    context->log().error() << "Unable to access memory block to store label";
    return ResultPtr(new Result());
  }

  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    context->log().error() << "Missing name for label";
    return ResultPtr(new Result());
  }

  return SymbolReferenceClass::createInstance(context, context->getProject()->getMemoryAdapter()->createReference(args.front()->convertToString(context)));
}

// ----------------------------------------------------------------------------
ResultPtr DataBlockClass::label(const ContextPtr &context, const GroupedParameterList &parameter) {
  MemoryBlock *block = memoryBlock(context);
  if (!block) {
    context->log().error() << "Unable to access memory block to store label";
    return ResultPtr(new Result());
  }

  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    return ResultPtr(new Result());
  }

  const auto &ref = SymbolReferenceClass::reference(args.front());
  if (!ref->isSymbolReference()) {
    context->log().error() << "Invalid argument for label";
    return ResultPtr(new Result());
  }

  const auto &sym = std::dynamic_pointer_cast<SymbolReference>(ref);
  block->reference(sym);
  
  return ResultPtr(new Result());
}


// ----------------------------------------------------------------------------
ResultPtr DataBlockClass::writeData(const ContextPtr &context, const GroupedParameterList &parameter, uint8_t itemSize) {
  MemoryBlock *block = memoryBlock(context);
  if (!block) {
    return ResultPtr(new Result());
  }

  const TypeMap &map = parameter.only_kwargs();
  TypeMap::const_iterator it;
  TypePtr fillByte(NumberClass::createInstance(context, 0)->get());
  uint32_t maxNumBytes = 0;

  if ((it = map.find("length")) != map.end()) {
    maxNumBytes = it->second->convertToNumber(context);
  }
  if ((it = map.find("fill")) != map.end()) {
    fillByte = convertToWriteableType(context, it->second);
  }

  for (auto &item : parameter.only_args()) {
    writeDataRecursive(block, context, item, itemSize, maxNumBytes);
  }

  while (maxNumBytes > 0) {
    writeDataRecursive(block, context, fillByte, itemSize, maxNumBytes);
  }

  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
void DataBlockClass::writeDataRecursive(MemoryBlock *block, const ContextPtr &context, const TypePtr &value, uint8_t itemSize, uint32_t &maxBytes) {
  ObjectPtr obj = convertToWriteableType(context, value);
  if (!obj) {
    return;
  }

  bool useMaxBytes = maxBytes > 0;

  if (obj->isObjectOfType("Number")) {
    uint64_t number = obj->convertToNumber(context);
    block->write(&number, itemSize);

    if (useMaxBytes) {
      maxBytes--;
    }
  } else if (obj->isObjectOfType("Array")) {
    const std::vector<TypePtr> &items = obj->convertToArray(context);
    for (auto &item : items) {
      writeDataRecursive(block, context, item, itemSize, maxBytes);
      
      if (useMaxBytes && maxBytes == 0) {
        return;
      }
    }
  } else if (obj->isObjectOfType("SymbolReference")) {
    const ReferencePtr &reference = SymbolReferenceClass::reference(obj);
    
    if (!reference) {
      context->log().error() << "SymbolReference has invalid symbol";
    } else {
      block->write(reference, itemSize);
    }
  } else {
    context->log().error() << "Invalid type to write: " << obj->asString();
  }
}

// ----------------------------------------------------------------------------
ObjectPtr DataBlockClass::convertToWriteableType(const ContextPtr &context, const TypePtr &value) {
  if (value->isObjectOfType("Number") || value->isObjectOfType("SymbolReference")) {
    return value->asObject();
  } else if (value->isObjectOfType("String")) {
    GroupedParameterList params;
    ContextPtr callContext(new InstanceContext(context->getInterpreter(), value->asObject(), "bytes"));
    return value->getMember("bytes")->call(callContext, params)->asObject();
  } else if (value->isObject() && value->hasMember("to_n")) {
    GroupedParameterList empty;
    ContextPtr callContext(new InstanceContext(context->getInterpreter(), value->asObject(), "to_n"));
    return convertToWriteableType(context, value->getMember("to_n")->call(callContext, empty)->get());
  }

  context->log().error() << "Unable to write data from type " << value->asString();
  return NumberClass::createInstance(context, 0)->asObject();
}

// ----------------------------------------------------------------------------
MemoryBlock* DataBlockClass::memoryBlock(const ContextPtr &context) {
  return memoryBlock(context->getProject(), context->self());
}

// ----------------------------------------------------------------------------
MemoryBlock* DataBlockClass::memoryBlock(const Project *project, const TypePtr &type) {
  MemoryBlock::Usage usage;

  if (type->isObjectOfType("DataBlock")) {
    usage = MemoryBlock::RANDOM_ACCESS;
  } else if (type->isObjectOfType("Function")) {
    usage = MemoryBlock::READ_ONLY;
  } else {
    project->log().error() << "Invalid call to memoryBlock on unsupported object type";
    return NULL;
  }

  MemoryBlock *block = MemoryBlock::getFromType(type);
  if (block) {
    return block;
  }

  block = project->getMemoryAdapter()->allocateBlock(usage, "");
  block->declareInType(type);
  block->markIsUsed(false);
  return block;
}
