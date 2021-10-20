#include <superfx/InstructionArguments.hpp>
#include <fma/interpret/ProjectContext.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/assem/ConstantNumberOperand.hpp>
#include <fma/assem/SymbolReferenceOperand.hpp>
#include <fma/assem/MemoryAddressOperand.hpp>
#include <fma/assem/DirectPageOperand.hpp>
#include <fma/assem/RelativeAddressOperand.hpp>
#include <fma/assem/LocalAddressOperand.hpp>
#include <fma/assem/StackAddressOperand.hpp>
#include <fma/assem/RegisterOperand.hpp>
#include <fma/types/Object.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/types/Class.hpp>

#include <iostream>

using namespace FMA;
using namespace FMA::types;
using namespace FMA::assem;
using namespace FMA::interpret;
using namespace FMA::superfx;

// ----------------------------------------------------------------------------
InstructionArguments::InstructionArguments(const ContextPtr &context, Project *project, const GroupedParameterList &_params)
  : left(NULL)
  , right(NULL)
  , context(context)
  , project(project)
  , valid(true)
{
  const TypeList &params = _params.only_args();
  numArgs = params.size();

  if (numArgs == 1) {
    left = analyzeArgs(params.front());
  } else if (numArgs == 2) {
    left = analyzeArgs(params.front());
    right = analyzeArgs(params.back());
  }
}

// ----------------------------------------------------------------------------
InstructionArguments::~InstructionArguments() {
  delete left;
  delete right;
}

// ----------------------------------------------------------------------------
InstructionArgument *InstructionArguments::analyzeArgs(const TypePtr &param) {
  const ObjectPtr &value = param->asObject();

  if (!value) {
    valid = false;
    return NULL;
  }

  ClassPtr ImmediateNumber = project->root()->getMember("ImmediateNumber")->asClass();
  ClassPtr TypedNumber = project->root()->getMember("TypedNumber")->asClass();
  ClassPtr Number = project->root()->getMember("Number")->asClass();
  ClassPtr Register = project->root()->getMember("Register")->asClass();
  ClassPtr SymbolReference = project->root()->getMember("SymbolReference")->asClass();

  const ClassPtr &self = value->getClass();

  if (self->isInstanceOf(ImmediateNumber)) {
    return analyzeImmediate(value);
  } else if (self->isInstanceOf(Number)) {
    return analyzeNumber(value);
  } else if (self->isInstanceOf(SymbolReference)) {
    return analyzeSymbol(value);
  } else if (self->isInstanceOf(TypedNumber)) {
    return analyzeTypedNumber(value);
  } else if (self->isInstanceOf(Register)) {
    return analyzeRegister(value);
  } else if (value->hasMember("to_sym")) {
    GroupedParameterList empty;
    return analyzeArgs(value->callDirect("to_sym", context, empty)->get());
  } else {
    std::cout << "UNKNOWN ARG " << value->asString() << std::endl;
    valid = false;
  }

  return NULL;
}

// ----------------------------------------------------------------------------
InstructionArgument *InstructionArguments::analyzeImmediate(const ObjectPtr &value) {
  InstructionArgument *arg = analyzeArgs(value->getMember("number"));
  if (arg == NULL || arg->type != InstructionArgument::ADDRESS) {
    delete arg;
    valid = false;
    return arg;
  }

  arg->type = InstructionArgument::IMMEDIATE;
  return arg;
}

// ----------------------------------------------------------------------------
InstructionArgument *InstructionArguments::analyzeNumber(const ObjectPtr &value) {
  TypePtr valueObject = value->getMember("__value");
  if (!valueObject->isInternalObjectOfType("Number")) {
    valid = false;
    return NULL;
  }

  InternalNumberValue *number = dynamic_cast<InternalNumberValue*>(valueObject.get());
  return new InstructionArgument(
    InstructionArgument::ADDRESS,
    number->getValue()
  );
}

// ----------------------------------------------------------------------------
InstructionArgument *InstructionArguments::analyzeTypedNumber(const ObjectPtr &value) {
  InstructionArgument *inner = analyzeArgs(value->getMember("number"));
  TypePtr valueObject = value->getMember("type")->getMember("__value");
  if (!valueObject->isInternalObjectOfType("String")) {
    valid = false;
    return NULL;
  }

  valid = false;
  delete inner;
  return NULL;
}

// ----------------------------------------------------------------------------
InstructionArgument *InstructionArguments::analyzeRegister(const ObjectPtr &value) {
  TypePtr valueObject = value->getMember("name")->getMember("__value");
  if (!valueObject->isInternalObjectOfType("String")) {
    valid = false;
    return NULL;
  }

  valid = false;
  return NULL;
}

// ----------------------------------------------------------------------------
InstructionArgument *InstructionArguments::analyzeSymbol(const ObjectPtr &value) {
  TypePtr valueObject = value->getMember("__value");
  if (!valueObject->isInternalObjectOfType("SymbolReference")) {
    valid = false;
    return NULL;
  }

  InternalReferenceValue *reference = dynamic_cast<InternalReferenceValue*>(valueObject.get());
  return new InstructionArgument(
    InstructionArgument::ADDRESS,
    reference->getValue()
  );
}

// ----------------------------------------------------------------------------