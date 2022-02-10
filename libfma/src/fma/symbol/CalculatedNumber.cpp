#include <fma/symbol/CalculatedNumber.hpp>
#include <fma/output/DynamicBuffer.hpp>
#include <sstream>

using namespace FMA::symbol;

// ----------------------------------------------------------------------------
std::string CalculatedNumber::operandAsString() const {
  switch (op) {
    case CalculatedNumber::ADD: return "+";
    case CalculatedNumber::SUB: return "-";
    case CalculatedNumber::DIV: return "/";
    case CalculatedNumber::MUL: return "*";
    case CalculatedNumber::REM: return "%";
    case CalculatedNumber::AND: return "&";
    case CalculatedNumber::OR: return "|";
    case CalculatedNumber::XOR: return "^";
    case CalculatedNumber::LSHIFT: return "<<";
    case CalculatedNumber::RSHIFT: return ">>";
    case CalculatedNumber::LESS_THAN: return "<";
    case CalculatedNumber::GREATER_THAN: return ">";
    case CalculatedNumber::LESS_OR_EQUAL_THAN: return "<=";
    case CalculatedNumber::GREATER_OR_EQUAL_THAN: return ">=";
    default: return "???";
  }
}

// ----------------------------------------------------------------------------
std::string CalculatedNumber::asString() const {
  std::ostringstream os;
  os << left->asString();
  os << operandAsString();
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
    case CalculatedNumber::GREATER_THAN: return (leftValue > rightValue) ? 1 : 0;
    case CalculatedNumber::LESS_THAN: return (leftValue < rightValue) ? 1 : 0;
    case CalculatedNumber::GREATER_OR_EQUAL_THAN: return (leftValue >= rightValue) ? 1 : 0;
    case CalculatedNumber::LESS_OR_EQUAL_THAN: return (leftValue <= rightValue) ? 1 : 0;

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
