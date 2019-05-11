#include <memory/MemoryLocationConstraint.hpp>
#include <memory/MemoryLocationList.hpp>
#include <memory/MemoryMap.hpp>
#include <memory/MemoryMapBank.hpp>
#include <fma/types/Object.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace FMA::memory;
using namespace FMA::interpret;
using namespace FMA::types;

// ----------------------------------------------------------------------------
MemoryLocationConstraint::MemoryLocationConstraint()
  : _alignment(0)
  , _alignmentOffset(0)
  , _isAllow(false)
{
}

// ----------------------------------------------------------------------------
MemoryLocationConstraint::MemoryLocationConstraint(bool isAllow)
  : _alignment(0)
  , _alignmentOffset(0)
  , _isAllow(isAllow)
{
}

// ----------------------------------------------------------------------------
MemoryLocationConstraint::MemoryLocationConstraint(const ContextPtr &context, const GroupedParameterList &params, bool isAllow)
  : _alignment(0)
  , _alignmentOffset(0)
  , _isAllow(isAllow)
{
  configure(context, params);
}

// ----------------------------------------------------------------------------
MemoryLocationConstraint::~MemoryLocationConstraint() {

}

// ----------------------------------------------------------------------------
void MemoryLocationConstraint::configure(const ContextPtr &context, const GroupedParameterList &params) {
  const TypeMap &kwargs = params.only_kwargs();
  TypeMap::const_iterator it;

  if ((it = kwargs.find("bank")) != kwargs.end()) {
    const TypePtr &value = it->second;
    if (value->isObjectOfType("Range")) {
      const auto &range = value->convertToRange(context);
      banks(range.first, range.last);
    } else {
      bank(value->convertToNumber(context));
    }
  }

  it = kwargs.find("at");
  if (it == kwargs.end()) it = kwargs.find("address");
  if (it != kwargs.end()) {
    const TypePtr &value = it->second;
    if (value->isObjectOfType("Range")) {
      const auto &_range = value->convertToRange(context);
      range(_range.first, _range.last);
    } else {
      address(value->convertToNumber(context));
    }
  }

  if ((it = kwargs.find("align")) != kwargs.end()) {
    alignedTo(it->second->convertToNumber(context));
  }
}

// ----------------------------------------------------------------------------
MemoryLocationConstraint &MemoryLocationConstraint::anyAlignment() {
  _alignment = 0;
  _alignmentOffset = 0;
  return *this;
}

// ----------------------------------------------------------------------------
MemoryLocationConstraint &MemoryLocationConstraint::alignedTo(uint32_t alignment, uint32_t offset) {
  _alignment = alignment;
  _alignmentOffset = offset;
  return *this;
}

// ----------------------------------------------------------------------------
bool MemoryLocationConstraint::alignmentMatches(uint64_t address) const {
  if (_alignment == 0) {
    return true;
  }

  return ((address - _alignmentOffset) % _alignment) == 0;
}

// ----------------------------------------------------------------------------
void MemoryLocationConstraint::overrideBy(const MemoryLocationConstraint &other) {
  if (other.banks().size()) {
    _banks = other.banks();
  }

  if (other.ranges().size()) {
    _ranges = other.ranges();
  }

  if (other.addresses().size()) {
    _addresses = other.addresses();
    anyRange();
  }

  if (other._alignment) {
    _alignment = other._alignment;
    _alignmentOffset = other._alignmentOffset;
  }
}

// ----------------------------------------------------------------------------
bool MemoryLocationConstraint::isEmptyConstraint() const {
  return !_banks.size() && !_addresses.size() && _alignment == 0 && !_ranges.size();
}

// ----------------------------------------------------------------------------
MemoryLocationConstraint &MemoryLocationConstraint::anyBank() {
  _banks.clear();
  return *this;
}

// ----------------------------------------------------------------------------
MemoryLocationConstraint &MemoryLocationConstraint::bank(MemoryBankIndex bank) {
  _banks.push_back(bank);
  return *this;
}

// ----------------------------------------------------------------------------
MemoryLocationConstraint &MemoryLocationConstraint::banks(MemoryBankIndex from, MemoryBankIndex to) {
  uint32_t index = from;
  while (index <= to) {
    bank(index++);
  }

  return *this;
}

// ----------------------------------------------------------------------------
MemoryLocationConstraint &MemoryLocationConstraint::anyRange() {
  _ranges.clear();
  return *this;
}

// ----------------------------------------------------------------------------
MemoryLocationConstraint &MemoryLocationConstraint::anyAddress() {
  _addresses.clear();
  _ranges.clear();
  return *this;
}

// ----------------------------------------------------------------------------
MemoryLocationConstraint &MemoryLocationConstraint::address(const MemoryBankSize &address) {
  _addresses.push_back(address);
  return *this;
}

// ----------------------------------------------------------------------------
MemoryLocationConstraint &MemoryLocationConstraint::range(const MemoryBankSize &from, const MemoryBankSize &to) {
  _ranges.push_back(MemoryLocationRange(from, to));
  return *this;
}

// ----------------------------------------------------------------------------
bool MemoryLocationConstraint::isStaticAddress() const {
  return _addresses.size();
}

// ----------------------------------------------------------------------------
bool MemoryLocationConstraint::isStaticBank() const {
  return _banks.size();
}

// ----------------------------------------------------------------------------
void MemoryLocationConstraint::mergeRemoveAddressesForRange(const MemoryLocationConstraint &, std::vector<MemoryLocationConstraint> &result) const {
  if (!_addresses.size() || !_ranges.size()) {
    result.push_back(*this);
    return;
  }

  MemoryLocationConstraint copy(*this);
  copy.anyAddress();

  for (auto &address : _addresses) {
    bool isValid = false;
    for (auto &range : _ranges) {
      if (address >= range.from && address <= range.to) {
        isValid = true;
        break;
      }
    }

    if (isValid) {
      copy.address(address);
    }
  }

  if (!copy._addresses.size()) {
    // no match between ranges and 
    return;
  }

  result.push_back(copy);
}

// ----------------------------------------------------------------------------
void MemoryLocationConstraint::mergeAlign(const MemoryLocationConstraint &other, std::vector<MemoryLocationConstraint> &result) const {
  if (!other.hasAlignment()) {
    mergeRemoveAddressesForRange(other, result);
    return;
  } else if (!hasAlignment()) {
    MemoryLocationConstraint copy(*this);
    copy._alignment = other._alignment;
    copy._alignmentOffset = other._alignmentOffset;
    copy.mergeRemoveAddressesForRange(other, result);
    return;
  }

  std::cout << "TODO: BOTH ALIGN: " << asString() << " AND " << other.asString() << std::endl;
}

// ----------------------------------------------------------------------------
void MemoryLocationConstraint::mergeRanges(const MemoryLocationConstraint &other, std::vector<MemoryLocationConstraint> &result) const {
  if (!other.ranges().size()) {
    mergeAlign(other, result);
    return;
  } else if (!_ranges.size() && other.ranges().size()) {
    MemoryLocationConstraint copy(*this);
    copy._ranges = other.ranges();
    copy.mergeAlign(other, result);
    return;
  }

  MemoryLocationConstraint copy(*this);
  copy.anyRange();
  for (auto &left : _ranges) {
    for (auto &right : other.ranges()) {
      MemoryBankSize from = left.from > right.from ? left.from : right.from;
      MemoryBankSize to = left.to < right.to ? left.to : right.to;

      if (from > to) {
        continue;
      }

      copy.range(from, to);
    }
  }

  if (!copy._ranges.size()) {
    // no matches between ranges
    return;
  }

  copy.mergeAlign(other, result);
}

// ----------------------------------------------------------------------------
void MemoryLocationConstraint::mergeAddresses(const MemoryLocationConstraint &other, std::vector<MemoryLocationConstraint> &result) const {
  if (!other._addresses.size()) {
    mergeRanges(other, result);
    return;
  } else if (other.addresses().size() && !_addresses.size()) {
    MemoryLocationConstraint copy(*this);
    copy._addresses = other.addresses();
    copy.mergeRanges(other, result);
    return;
  }

  std::cout << "TODO: BOTH ADDRESSES: " << asString() << " AND " << other.asString() << std::endl;
}

// ----------------------------------------------------------------------------
void MemoryLocationConstraint::mergeBanks(const MemoryLocationConstraint &other, std::vector<MemoryLocationConstraint> &result) const {
  MemoryLocationConstraint copy(*this);
  copy._banks = other.banks();

  copy.mergeAddresses(other, result);
}

// ----------------------------------------------------------------------------
void MemoryLocationConstraint::merge(const MemoryLocationConstraint &other, std::vector<MemoryLocationConstraint> &result) const {
  if (!_banks.size()) {
    mergeBanks(other, result);
    return;
  } else if (!other.banks().size()) {
    other.mergeBanks(*this, result);
    return;
  }

  MemoryLocationConstraint newItem(*this);
  newItem.anyBank();

  bool newItemIsValid = false;
  for (auto &myBank : _banks) {
    for (auto &otherBank : other.banks()) {
      if (myBank != otherBank) {
        continue;
      }

      newItem.bank(myBank);
      newItemIsValid = true;
    }
  }

  if (newItemIsValid) {
    newItem.mergeAddresses(other, result);
  }
}

// ----------------------------------------------------------------------------
void MemoryLocationConstraint::applySingleBankMemoryShadow(MemoryBankIndex index, MemoryMap *map, MemoryLocationList &newList) {
  MemoryMapBank *bank = map->getBank(index);
  if (!bank) {
    return;
  }

  if (!bank->hasAnyShadow()) {
    newList.add(*this);
    return;
  }

  std::cout << "TODO: BANK!!!!! MAPP!!!" << std::endl;
  bank->dump();
}

// ----------------------------------------------------------------------------
void MemoryLocationConstraint::applyMemoryShadows(MemoryMap *map, MemoryLocationList &newList) const {
  if (!_banks.size()) {
    newList.add(*this);
    return;
  }

  for (auto &bank : _banks) {
    MemoryLocationConstraint copy(*this);
    copy.anyBank();
    copy.applySingleBankMemoryShadow(bank, map, newList);
  }
}

// ----------------------------------------------------------------------------
std::string MemoryLocationConstraint::asString() const {
  std::ostringstream os;
  os << "{";
  bool firstMain=true;

  if (_banks.size()) {
    os << "BANK(";
    bool first=true;
    for (const auto &bank : _banks) {
      if (!first) os << ",";
      else first = false;
      os << "$" << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << (int)bank << std::nouppercase;
    }
    os << std::dec << ")";
    firstMain = false;
  }

  if (_addresses.size()) {
    if (!firstMain) os << ",";
    bool first=true;
    os << "AT(";
    for (const auto &address: _addresses) {
      if (!first) os << ",";
      else first = false;
      os << "$" << std::setw(4) << std::setfill('0') << std::hex << std::uppercase << address<< std::nouppercase;
    }
    os << std::dec << ")";
    firstMain = false;
  }

  if (_ranges.size()) {
    if (!firstMain) os << ",";
    bool first=true;
    os << "RANGE(";
    for (const auto &address: _ranges) {
      if (!first) os << ",";
      else first = false;
      os << "$" << std::setw(4) << std::setfill('0') << std::hex << std::uppercase << address.from << "..$" << address.to << std::nouppercase;
    }
    os << std::dec << ")";
    firstMain = false;
  }

  if (_alignment != 0) {
    if (!firstMain) os << ",";
    os << "ALIGN($" << std::setw(4) << std::setfill('0') << std::hex << std::uppercase << _alignment;
    if (_alignmentOffset > 0) {
      os << "+$" << _alignmentOffset;
    }
    os << ")";

    firstMain = false;
  }

  os << "}";
  return os.str();
}