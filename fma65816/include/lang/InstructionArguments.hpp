#ifndef __FMA65816_LANG_INSTRUCTIONARGUMENTS_H__
#define __FMA65816_LANG_INSTRUCTIONARGUMENTS_H__

#include <fma/interpret/ParameterList.hpp>
#include <fma/types/Base.hpp>
#include <fma/symbol/Reference.hpp>
#include <fma/assem/Operand.hpp>
#include <fma/Project.hpp>

namespace FMA65816 {
namespace lang {

struct InstructionArgument {
  enum Type {
    INVALID,
    INDIRECT,
    INDIRECT_X,
    IMMEDIATE,
    DIRECT_PAGE,
    LONG_ADDRESS,
    LONG_INDIRECT,
    REG_A,
    REG_X,
    REG_Y,
    REG_S,
    ADDRESS
  };

  enum ValueType {
    VALUE_NONE,
    VALUE_NUMBER,
    VALUE_SYMBOL
  };

  InstructionArgument()
    : type(INVALID), valueType(VALUE_NONE) {}
  InstructionArgument(Type type)
    : type(type), valueType(VALUE_NONE) {}
  InstructionArgument(Type type, const uint64_t &number)
    : type(type), valueType(VALUE_NUMBER), number(number) {}
  InstructionArgument(Type type, const FMA::symbol::ReferencePtr &reference)
    : type(type), valueType(VALUE_SYMBOL), reference(reference) {}
  
  ~InstructionArgument() {
  }

  FMA::assem::Operand *createOperand(InstructionArgument *other=NULL) const;
  FMA::assem::Operand *createValueOperand(InstructionArgument *other) const;
  FMA::assem::Operand *createValueOperand() const;

  Type type;
  ValueType valueType;
  uint64_t number;
  FMA::symbol::ReferencePtr reference;
};

class InstructionArguments {
public:
  InstructionArguments(const FMA::interpret::ContextPtr &context, FMA::Project *project, const FMA::interpret::GroupedParameterList &params);
  ~InstructionArguments();

  inline bool isValid() const { return valid; }

  inline bool absolute() const { return numArgs == 1 && left->type==InstructionArgument::ADDRESS; }
  inline bool immediate() const { return numArgs == 1 && left->type==InstructionArgument::IMMEDIATE; }
  inline bool move() const { return numArgs == 2 && left->type==InstructionArgument::IMMEDIATE && right->type==InstructionArgument::IMMEDIATE; }
  inline bool indirect() const { return numArgs == 1 && left->type==InstructionArgument::INDIRECT; }
  inline bool longIndirect() const { return numArgs == 1 && left->type==InstructionArgument::LONG_INDIRECT; }
  inline bool indirectY() const { return numArgs == 2 && left->type==InstructionArgument::INDIRECT && right->type==InstructionArgument::REG_Y; }
  inline bool longIndirectY() const { return numArgs == 2 && left->type==InstructionArgument::LONG_INDIRECT && right->type==InstructionArgument::REG_Y; }
  inline bool directPage() const { return numArgs == 1 && left->type==InstructionArgument::DIRECT_PAGE; }
  inline bool longAddress() const { return numArgs == 1 && left->type==InstructionArgument::LONG_ADDRESS; }
  inline bool absoluteX() const { return numArgs == 2 && left->type==InstructionArgument::ADDRESS && right->type==InstructionArgument::REG_X; }
  inline bool absoluteY() const { return numArgs == 2 && left->type==InstructionArgument::ADDRESS && right->type==InstructionArgument::REG_Y; }
  inline bool absoluteS() const { return numArgs == 2 && left->type==InstructionArgument::ADDRESS && right->type==InstructionArgument::REG_S; }
  inline bool absoluteLongX() const { return numArgs == 2 && left->type==InstructionArgument::LONG_ADDRESS && right->type==InstructionArgument::REG_X; }
  inline bool absoluteLongY() const { return numArgs == 2 && left->type==InstructionArgument::LONG_ADDRESS && right->type==InstructionArgument::REG_Y; }
  inline bool directPageX() const { return numArgs == 2 && left->type==InstructionArgument::DIRECT_PAGE && right->type==InstructionArgument::REG_X; }
  inline bool directPageY() const { return numArgs == 2 && left->type==InstructionArgument::DIRECT_PAGE && right->type==InstructionArgument::REG_Y; }
  inline bool indirectX() const { return numArgs == 2 && left->type==InstructionArgument::INDIRECT && right->type==InstructionArgument::REG_X; }
  inline bool registerA() const { return numArgs == 1 && left->type==InstructionArgument::REG_A; }
  inline bool registerX() const { return numArgs == 1 && left->type==InstructionArgument::REG_X; }
  inline bool registerY() const { return numArgs == 1 && left->type==InstructionArgument::REG_Y; }
  inline bool implicit() const { return numArgs == 0; }

  inline FMA::assem::Operand *createLeftOperand() const { return left->createOperand(); }
  inline FMA::assem::Operand *createRightOperand() const { return right->createOperand(); }
  FMA::assem::Operand *createOperand() const;

  inline InstructionArgument *getLeft() const { return left; }
  inline InstructionArgument *getRight() const { return right; }
 
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

#endif
