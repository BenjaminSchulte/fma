#ifndef __FMA_ASM_INSTRUCTION_H__
#define __FMA_ASM_INSTRUCTION_H__

#include <string>
#include <map>
#include <vector>
#include <boost/any.hpp>
#include "Operand.hpp"

namespace FMA {
namespace assem {

class Simulation;

typedef std::map<std::string, boost::any> InstructionAttributes;

class Instruction {
public:
  Instruction();
  virtual ~Instruction();

  virtual std::string getName() = 0;
  virtual std::vector<Operand*> getOperands() = 0;
  virtual std::string asString();
  virtual Operand* getOperand(uint8_t) = 0;
  virtual Operand *getNumericOperand() = 0;

  virtual std::string getIdentifier();

  virtual bool containsBinaryData() const { return false; }
  virtual void appendBinaryData(const void*, uint32_t) {}

  virtual const boost::any &getAttribute(const std::string &);
  virtual void setAttribute(const std::string &, const boost::any &value);
  virtual bool hasAttribute(const std::string &);

protected:
  InstructionAttributes *attributes;
};

class Instruction0 : public Instruction {
public:
  Instruction0() {}
  virtual ~Instruction0() {}

  virtual std::vector<Operand*> getOperands();
  virtual Operand* getOperand(uint8_t);
  virtual Operand *getNumericOperand() { return NULL; }
};

class Instruction1 : public Instruction {
public:
  Instruction1(Operand *op) : op(op) {}
  virtual ~Instruction1() { delete op; }

  virtual std::vector<Operand*> getOperands();
  virtual Operand* getOperand(uint8_t);
  virtual Operand *getNumericOperand() { return op; }
  
  Operand *op;
};

class Instruction2 : public Instruction {
public:
  Instruction2(Operand *left, Operand *right) : left(left), right(right) {}
  virtual ~Instruction2() { delete left; delete right; }
  
  virtual std::vector<Operand*> getOperands();
  virtual Operand* getOperand(uint8_t);
  virtual Operand *getNumericOperand();
  
  Operand *left;
  Operand *right;
};

class Instruction3 : public Instruction {
public:
  Instruction3(Operand *left, Operand *mid, Operand *right) : left(left), mid(mid), right(right) {}
  virtual ~Instruction3() { delete left; delete mid; delete right; }
  
  virtual std::vector<Operand*> getOperands();
  virtual Operand* getOperand(uint8_t);
  virtual Operand *getNumericOperand();
  
  Operand *left;
  Operand *mid;
  Operand *right;
};

}
}

#endif