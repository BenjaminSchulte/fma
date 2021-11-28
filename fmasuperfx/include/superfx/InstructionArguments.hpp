#pragma once

#include <fma/interpret/ParameterList.hpp>
#include <fma/types/Base.hpp>
#include <fma/symbol/Reference.hpp>
#include <fma/assem/Operand.hpp>
#include <fma/Project.hpp>

namespace FMA {
namespace superfx {

enum class ArgumentsType {
  IMPLICIT,
  INVALID
};

struct InstructionArgument {
  enum Type {
    INVALID,
    IMMEDIATE,
    ADDRESS,
    REGISTER
  };

  enum ValueType {
    VALUE_NONE,
    VALUE_NUMBER,
    VALUE_SYMBOL
  };

  InstructionArgument() : type(INVALID), valueType(VALUE_NONE) {}
  InstructionArgument(Type type)
    : type(type), valueType(VALUE_NONE) {}
  InstructionArgument(Type type, const uint64_t &number)
    : type(type), valueType(VALUE_NUMBER), number(number) {}
  InstructionArgument(Type type, const FMA::symbol::ReferencePtr &reference)
    : type(type), valueType(VALUE_SYMBOL), reference(reference) {}
  
  Type type;
  ValueType valueType;
  uint64_t number;
  FMA::symbol::ReferencePtr reference;
};

class InstructionArguments {
public:

  InstructionArguments(const FMA::interpret::ContextPtr &context, FMA::Project *project, const FMA::interpret::GroupedParameterList &params);
  ~InstructionArguments();

  ArgumentsType type() const;

  inline InstructionArgument *getLeft() const { return left; }
  inline InstructionArgument *getRight() const { return right; }

  inline bool isValid() const { return valid; }

  inline bool implicit() const { return numArgs == 0; }
  inline bool immediate() const { return numArgs == 1 && left->type==InstructionArgument::IMMEDIATE; }
  inline bool address() const { return numArgs == 1 && left->type==InstructionArgument::ADDRESS; }
  inline bool reg() const { return numArgs == 1 && left->type==InstructionArgument::REGISTER; }
  inline bool regImmediate() const { return numArgs == 2 && left->type==InstructionArgument::REGISTER && right->type==InstructionArgument::IMMEDIATE; }
  inline bool regAddress() const { return numArgs == 2 && left->type==InstructionArgument::REGISTER && right->type==InstructionArgument::ADDRESS; }
  inline bool regReg() const { return numArgs == 2 && left->type==InstructionArgument::REGISTER && right->type==InstructionArgument::REGISTER; }
 
protected:
  InstructionArgument *analyzeArgs(const FMA::types::TypePtr &param);

  InstructionArgument *analyzeImmediate(const FMA::types::ObjectPtr &value);
  InstructionArgument *analyzeNumber(const FMA::types::ObjectPtr &value);
  InstructionArgument *analyzeTypedNumber(const FMA::types::ObjectPtr &value);
  InstructionArgument *analyzeSymbol(const FMA::types::ObjectPtr &value);
  InstructionArgument *analyzeRegister(const FMA::types::ObjectPtr &value);

  InstructionArgument *left;
  InstructionArgument *right;

  FMA::interpret::ContextPtr context;
  FMA::Project *project;
  bool valid;
  uint8_t numArgs;
};

}
}

