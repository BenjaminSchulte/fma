#include <lang/InstructionArguments.hpp>
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
using namespace FMA65816::lang;

// ----------------------------------------------------------------------------
Operand *InstructionArgument::createOperand(InstructionArgument *other) const {
  switch (type) {
  case IMMEDIATE:
    return createValueOperand(other);

  case ADDRESS:
    return new MemoryAddressOperand(new LocalAddressOperand(createValueOperand(other)));

  case INDIRECT:
    return new MemoryAddressOperand(new LocalAddressOperand(new MemoryAddressOperand(new DirectPageOperand(createValueOperand(other)))));

  case LONG_INDIRECT:
    return new MemoryAddressOperand(new MemoryAddressOperand(new DirectPageOperand(createValueOperand(other))));

  case DIRECT_PAGE:
    return new MemoryAddressOperand(new DirectPageOperand(createValueOperand(other)));

  case LONG_ADDRESS:
    return new MemoryAddressOperand(createValueOperand(other));

  case REG_A:
    return new RegisterOperand("A");

  case REG_X:
    return new RegisterOperand("X");

  case REG_Y:
    return new RegisterOperand("Y");

  case REG_S:
    return new RegisterOperand("S");

  default:
    return createValueOperand();
  }

  return NULL;
}

// ----------------------------------------------------------------------------
Operand *InstructionArgument::createValueOperand(InstructionArgument *other) const {
  Operand *operand = createValueOperand();

  if (other != NULL) {
    operand = new RelativeAddressOperand(operand, other->createOperand());
  }

  return operand;
}

// ----------------------------------------------------------------------------
Operand *InstructionArgument::createValueOperand() const {
  switch (valueType) {
  case VALUE_NUMBER:
    return new ConstantNumberOperand(number);

  case VALUE_SYMBOL:
    return new SymbolReferenceOperand(reference);

  default:
    return new ConstantNumberOperand(0);
  }
}


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
FMA::assem::Operand *InstructionArguments::createOperand() const {
  if (absoluteS()) {
    return new MemoryAddressOperand(new StackAddressOperand(left->createValueOperand(right)));
  } else if (indirectY() || indirectX()) {
    return new MemoryAddressOperand(new RelativeAddressOperand(new LocalAddressOperand(new MemoryAddressOperand(new DirectPageOperand(left->createOperand()))), right->createOperand()));
  } else if (longIndirectY()) {
    return new MemoryAddressOperand(new RelativeAddressOperand(new MemoryAddressOperand(new DirectPageOperand(left->createOperand())), right->createOperand()));
  } else {
    return left->createOperand(right);
  }
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

  std::string typeName = dynamic_cast<InternalStringValue*>(valueObject.get())->getValue();
  if (typeName == "dp") {
    if (inner->type == InstructionArgument::ADDRESS) {
      inner->type = InstructionArgument::DIRECT_PAGE;
      return inner;
    }
  } else if (typeName == "long_address") {
    if (inner->type == InstructionArgument::ADDRESS) {
      inner->type = InstructionArgument::LONG_ADDRESS;
      return inner;
    } else if (inner->type == InstructionArgument::INDIRECT) {
      inner->type = InstructionArgument::LONG_INDIRECT;
      return inner;
    }
  } else if (typeName == "indirect") {
    TypePtr relativeTo = value->getMember("relative_to");
    if (relativeTo->isUndefined()) {
      if (inner->type == InstructionArgument::ADDRESS) {
        inner->type = InstructionArgument::INDIRECT;
        return inner;
      } else if (inner->type == InstructionArgument::LONG_ADDRESS) {
        inner->type = InstructionArgument::LONG_INDIRECT;
        return inner;
      }
    } else {
      InstructionArgument *outer = analyzeArgs(relativeTo);
      if (outer != NULL && outer->type == InstructionArgument::REG_X) {
        right = outer;
        inner->type = InstructionArgument::INDIRECT;
        return inner;
      }
    }
  } else if (typeName == "register_member_X") {
    numArgs = 2;
    right = new InstructionArgument(InstructionArgument::REG_X);
    return inner;
  } else if (typeName == "register_member_Y") {
    numArgs = 2;
    right = new InstructionArgument(InstructionArgument::REG_Y);
    return inner;
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

  std::string typeName = dynamic_cast<InternalStringValue*>(valueObject.get())->getValue();
  if (typeName == "A") {
    return new InstructionArgument(InstructionArgument::REG_A);
  } else if (typeName == "X") {
    return new InstructionArgument(InstructionArgument::REG_X);
  } else if (typeName == "Y") {
    return new InstructionArgument(InstructionArgument::REG_Y);
  } else if (typeName == "S") {
    return new InstructionArgument(InstructionArgument::REG_S);
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