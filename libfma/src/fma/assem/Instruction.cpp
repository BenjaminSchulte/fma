#include <fma/assem/Instruction.hpp>
#include <fma/assem/Operand.hpp>
#include <sstream>

using namespace FMA::assem;

namespace {
  boost::any boostNullPtr(nullptr);
}

// ----------------------------------------------------------------------------
Instruction::Instruction()
  : attributes(NULL)
{
}

// ----------------------------------------------------------------------------
Instruction::~Instruction() {
  if (attributes) {
    delete attributes;
  }
}

// ----------------------------------------------------------------------------
std::string Instruction::asString() {
  bool first = true;
  std::ostringstream os;
  std::vector<Operand *> operands = getOperands();
  os << getName();
  for (auto &operand : operands) {
    if (first) {
      os << " ";
      first = false;
    } else {
      os << ", ";
    }

    Operand *op = operand;
    os << op->asString();
  }

  return os.str();
}

// ----------------------------------------------------------------------------
std::string Instruction::getIdentifier() {
  bool first = true;
  std::ostringstream os;
  std::vector<Operand *> operands = getOperands();
  os << getName();
  for (auto &operand : operands) {
    if (first) {
      first = false;
      os << "(";
    } else {
      os << ",";
    }

    Operand *op = operand;
    os << op->getIdentifier();
  }

  if (!first) {
    os << ")";
  }
  
  return os.str();
}

// ----------------------------------------------------------------------------
const boost::any &Instruction::getAttribute(const std::string &name) {
  if (!attributes) {
    return boostNullPtr;
  }

  InstructionAttributes::const_iterator it = attributes->find(name);
  if (it == attributes->end()) {
    return boostNullPtr;
  }

  return it->second;
}

// ----------------------------------------------------------------------------
void Instruction::setAttribute(const std::string &name, const boost::any &value) {
  if (!attributes) {
    attributes = new InstructionAttributes();
  }

  (*attributes)[name] = value;
}

// ----------------------------------------------------------------------------
bool Instruction::hasAttribute(const std::string &name) {
  if (!attributes) {
    return false;
  }

  InstructionAttributes::const_iterator it = attributes->find(name);
  if (it == attributes->end()) {
    return false;
  }

  return true;
}



// ----------------------------------------------------------------------------
std::vector<Operand*> Instruction0::getOperands() {
  std::vector<Operand*> list;
  return list;
}

// ----------------------------------------------------------------------------
Operand* Instruction0::getOperand(uint8_t) {
  return NULL;
}

// ----------------------------------------------------------------------------
std::vector<Operand*> Instruction1::getOperands() {
  std::vector<Operand*> list;
  list.push_back(op);
  return list;
}

// ----------------------------------------------------------------------------
Operand* Instruction1::getOperand(uint8_t index) {
  if (index >= 1) {
    return NULL;
  }

  return op;
}


// ----------------------------------------------------------------------------
std::vector<Operand*> Instruction2::getOperands() {
  std::vector<Operand*> list;
  list.push_back(left);
  list.push_back(right);
  return list;
}

// ----------------------------------------------------------------------------
Operand* Instruction2::getOperand(uint8_t index) {
  switch (index) {
    case 0: return left;
    case 1: return right;

    default: return NULL;
  }
}

// ----------------------------------------------------------------------------
Operand* Instruction2::getNumericOperand() {
  if (left->isConstant() || left->isResolvedAddress() || left->isSymbolReference()) {
    return left;
  } else {
    return right;
  }
}


// ----------------------------------------------------------------------------
std::vector<Operand*> Instruction3::getOperands() {
  std::vector<Operand*> list;
  list.push_back(left);
  list.push_back(mid);
  list.push_back(right);
  return list;
}

// ----------------------------------------------------------------------------
Operand* Instruction3::getOperand(uint8_t index) {
  switch (index) {
    case 0: return left;
    case 1: return mid;
    case 2: return right;

    default: return NULL;
  }
}

// ----------------------------------------------------------------------------
Operand* Instruction3::getNumericOperand() {
  if (left->isConstant() || left->isResolvedAddress() || left->isSymbolReference()) {
    return left;
  } else if (mid->isConstant() || mid->isResolvedAddress() || mid->isSymbolReference()) {
    return mid;
  } else {
    return right;
  }
}
