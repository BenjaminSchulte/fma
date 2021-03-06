#include <fma/symbol/CalculatedNumber.hpp>
#include <fma/output/DynamicBuffer.hpp>
#include <sstream>

using namespace FMA::symbol;

// ----------------------------------------------------------------------------
std::string CalculatedNumber::asString() const {
  std::ostringstream os;
  os << left->asString();
  switch (op) {
    case CalculatedNumber::ADD: os << "+"; break;
    case CalculatedNumber::SUB: os << "-"; break;
    case CalculatedNumber::DIV: os << "/"; break;
    case CalculatedNumber::MUL: os << "*"; break;
    case CalculatedNumber::REM: os << "%"; break;
    case CalculatedNumber::AND: os << "&"; break;
    case CalculatedNumber::OR: os << "|"; break;
    case CalculatedNumber::XOR: os << "^"; break;
    case CalculatedNumber::LSHIFT: os << "<<"; break;
    case CalculatedNumber::RSHIFT: os << ">>"; break;
    default: os << "???"; break;
  }
  os << right->asString();
  return os.str();
}

// ----------------------------------------------------------------------------
bool CalculatedNumber::isConstant() const {
  return left->isConstant() && right->isConstant();
}

// ----------------------------------------------------------------------------
uint64_t CalculatedNumber::asConstant() const {
  return asConstant(left->asConstant(), right->asConstant());
}

// ----------------------------------------------------------------------------
uint64_t CalculatedNumber::asConstant(uint64_t leftValue, uint64_t rightValue) const {
  switch (op) {
    case CalculatedNumber::ADD: return leftValue + rightValue;
    case CalculatedNumber::SUB: return leftValue - rightValue;
    case CalculatedNumber::DIV: return leftValue / rightValue;
    case CalculatedNumber::MUL: return leftValue * rightValue;
    case CalculatedNumber::REM: return leftValue % rightValue;
    case CalculatedNumber::AND: return leftValue & rightValue;
    case CalculatedNumber::OR: return leftValue | rightValue;
    case CalculatedNumber::XOR: return leftValue ^ rightValue;
    case CalculatedNumber::LSHIFT: return leftValue << rightValue;
    case CalculatedNumber::RSHIFT: return leftValue >> rightValue;

    default:
      return 0;
  }
}

// ----------------------------------------------------------------------------
uint64_t CalculatedNumber::resolve(const plugin::MemorySymbolMap *map, bool &valid) const {
  uint64_t leftValue  = left->resolve(map, valid);
  uint64_t rightValue = right->resolve(map, valid);
  if (!valid) {
    return 0;
  }

  return asConstant(leftValue, rightValue);
}

// ----------------------------------------------------------------------------
bool CalculatedNumber::serialize(FMA::output::DynamicBuffer &buffer) const {
  uint16_t id = (uint16_t)SerializeReferenceId::CALCULATE_REFERENCE;
  uint8_t opId = op;

  buffer.write(&id, 2);
  buffer.write(&opId, 1);

  if (!left->serialize(buffer)
    || !right->serialize(buffer)
  ) {
    return false;
  }

  return true;
}

// ----------------------------------------------------------------------------
ReferencePtr CalculatedNumber::deserialize(Log *log, FMA::output::DynamicBuffer &buffer) {
  uint8_t opId;

  buffer.read(&opId, 1);
  ReferencePtr left(Reference::deserialize(log, buffer));
  ReferencePtr right(Reference::deserialize(log, buffer));

  return ReferencePtr(new CalculatedNumber(left, (Operand)opId, right));
}
