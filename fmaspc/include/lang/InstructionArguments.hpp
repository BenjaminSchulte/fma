#ifndef __FMASPC_LANG_INSTRUCTIONARGUMENTS_H__
#define __FMASPC_LANG_INSTRUCTIONARGUMENTS_H__

#include <fma/interpret/ParameterList.hpp>
#include <fma/types/Base.hpp>
#include <fma/symbol/Reference.hpp>
#include <fma/assem/Operand.hpp>
#include <fma/Project.hpp>

namespace FMASPC {
namespace lang {

struct InstructionArgument {
  enum Type {
    INVALID,
    DATA_FROM_X,
    INDIRECT,
    INDIRECT_X,
    INDIRECT_Y,
    DIRECT_PAGE,
    DP_X,
    ADDRESS,
    ADDRESS_X,
    ADDRESS_Y,
    IMMEDIATE,
    REG_A,
    FLAG_C,
    REG_X,
    REG_Y,
    REG_YA,
    REG_S,
    REG_PSW
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

  inline bool regA() const { return type == REG_A; }
  inline bool regX() const { return type == REG_X; }
  inline bool regY() const { return type == REG_Y; }
  inline bool regS() const { return type == REG_S; }
  inline bool regPSW() const { return type == REG_PSW; }
  inline bool regYA() const { return type == REG_YA; }
  inline bool flagC() const { return type == FLAG_C; }
  inline bool address() const { return type == ADDRESS; }
  inline bool addressX() const { return type == ADDRESS_X; }
  inline bool addressY() const { return type == ADDRESS_Y; }
  inline bool immediate() const { return type == IMMEDIATE; }
  inline bool indirect() const { return type == INDIRECT; }
  inline bool indirectX() const { return type == INDIRECT_X; }
  inline bool dataFromX() const { return type == DATA_FROM_X; }
  inline bool indirectY() const { return type == INDIRECT_Y; }
  inline bool dp() const { return type == DIRECT_PAGE; }
  inline bool dpX() const { return type == DP_X; }

  FMA::assem::Operand *createOperand(InstructionArgument *other=NULL) const;
  FMA::assem::Operand *createValueOperand(InstructionArgument *other) const;
  FMA::assem::Operand *createValueOperand() const;

  std::string asString() const;

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

  inline FMA::assem::Operand *createLeftOperand() const { return left->createOperand(); }
  inline FMA::assem::Operand *createRightOperand() const { return right->createOperand(); }
  inline FMA::assem::Operand *createThirdOperand() const { return add->createOperand(); }
  FMA::assem::Operand *createOperand() const;

  inline InstructionArgument *getLeft() const { return left; }
  inline InstructionArgument *getRight() const { return right; }
  inline InstructionArgument *getAdd() const { return add; }

  std::string asString() const;
 
protected:
  InstructionArgument *analyzeArgs(const FMA::types::TypePtr &param);

  InstructionArgument *analyzeImmediate(const FMA::types::ObjectPtr &value);
  InstructionArgument *analyzeNumber(const FMA::types::ObjectPtr &value);
  InstructionArgument *analyzeTypedNumber(const FMA::types::ObjectPtr &value);
  InstructionArgument *analyzeSymbol(const FMA::types::ObjectPtr &value);
  InstructionArgument *analyzeRegister(const FMA::types::ObjectPtr &value);

  InstructionArgument *left;
  InstructionArgument *right;
  InstructionArgument *add;

  FMA::interpret::ContextPtr context;
  FMA::Project *project;
  bool valid;
  uint8_t numArgs;
};

}
}

#endif
