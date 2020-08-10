#include <memory/MemoryLocationList.hpp>
#include <memory/MemoryLocationConstraint.hpp>
#include <fma/output/DynamicBuffer.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <iostream>

namespace {
  const uint16_t SERIALIZE_IDENTIFIER = 0xF8AE;
}

using namespace FMA::memory;
using namespace FMA::interpret;

// ----------------------------------------------------------------------------
void MemoryLocationList::allow(const ContextPtr &context, const GroupedParameterList &parameter) {
  MemoryLocationConstraint loc(context, parameter, true);

  if (!loc.isEmptyConstraint()) {
    allowed.push_back(loc);
  }
}

// ----------------------------------------------------------------------------
void MemoryLocationList::overrideAllow(const ContextPtr &context, const GroupedParameterList &parameter) {
  if (!allowed.size()) {
    return allow(context, parameter);
  }

  MemoryLocationConstraint loc(context, parameter, true);
  allowed.front().overrideBy(loc);
}

// ----------------------------------------------------------------------------
void MemoryLocationList::deny(const ContextPtr &context, const GroupedParameterList &parameter) {
  MemoryLocationConstraint loc(context, parameter, false);

  if (!loc.isEmptyConstraint()) {
    denied.push_back(loc);
  }
}

// ----------------------------------------------------------------------------
bool MemoryLocationList::isStaticAddressLocation() const {
  bool isStatic = false;
  for (auto &address : allowed) {
    if (address.isStaticAddress()) {
      isStatic = true;
    } else {
      return false;
    }
  }
  return isStatic;
}

// ----------------------------------------------------------------------------
bool MemoryLocationList::isStaticBankLocation() const {
  bool isStatic = false;
  for (auto &address : allowed) {
    if (address.isStaticBank()) {
      isStatic = true;
    } else {
      return false;
    }
  }
  return isStatic;
}

// ----------------------------------------------------------------------------
void MemoryLocationList::add(const MemoryLocationConstraint &item) {
  if (item.isAllow()) {
    allowed.push_back(item);
  } else {
    denied.push_back(item);
  }
}

// ----------------------------------------------------------------------------
MemoryLocationList MemoryLocationList::applyMemoryShadows(class MemoryMap *map) const {
  MemoryLocationList newList;

  for (auto &allow : allowed) {
    allow.applyMemoryShadows(map, newList);
  }

  for (auto &deny : denied) {
    deny.applyMemoryShadows(map, newList);
  }

  return newList;
}

// ----------------------------------------------------------------------------
MemoryLocationList MemoryLocationList::merge(const MemoryLocationList &other) const {
  MemoryLocationList result;
  result.allowed = merge(allowed, other.allowed);
  result.denied = merge(denied, other.denied);
  return result;
}

// ----------------------------------------------------------------------------
MemoryLocationConstraints MemoryLocationList::merge(const MemoryLocationConstraints &left, const MemoryLocationConstraints &right) const {
  MemoryLocationConstraints result;
  //dbg// std::cout << "================================== MERGE ==================================" << std::endl;
  //dbg// for (const auto &dumpLeft : left) { std::cout << "* " << dumpLeft.asString() << std::endl; }
  //dbg// std::cout << "AND" << std::endl;
  //dbg// for (const auto &dumpRight : right) { std::cout << "* " << dumpRight.asString() << std::endl; }

  if (!left.size()) {
    return right;
  } else if (!right.size()) {
    return left;
  }

  for (const auto &itLeft : left) {
    for (const auto &itRight : right) {
      itLeft.merge(itRight, result);
    }
  }

  //dbg// std::cout << "IS" << std::endl;
  //dbg// for (const auto &dumpResult : result) { std::cout << "* " << dumpResult.asString() << std::endl; }
  //dbg// std::cout << "==================================" << std::endl;
  
  return result;
}

// ----------------------------------------------------------------------------
void MemoryLocationList::dump() const {
  for (auto &allow : allowed) {
    std::cout << "++ " << allow.asString() << std::endl;
  }
  for (auto &deny : denied) {
    std::cout << "-- " << deny.asString() << std::endl;
  }
}

// ----------------------------------------------------------------------------
bool MemoryLocationList::serialize(FMA::output::DynamicBuffer &buffer) const {
  buffer.write(&SERIALIZE_IDENTIFIER, sizeof(SERIALIZE_IDENTIFIER));

  uint16_t numAllowed = allowed.size();
  buffer.write(&numAllowed, 2);
  for (const MemoryLocationConstraint &c : allowed) {
    if (!c.serialize(buffer)) {
      return false;
    }
  }

  uint16_t numDenied = denied.size();
  buffer.write(&numDenied, 2);
  for (const MemoryLocationConstraint &c : denied) {
    if (!c.serialize(buffer)) {
      return false;
    }
  }

  return true;
}

// ----------------------------------------------------------------------------
bool MemoryLocationList::deserialize(Log *log, FMA::output::DynamicBuffer &buffer) {
  uint16_t header;
  if (!buffer.read(&header, sizeof(header)) || header != SERIALIZE_IDENTIFIER) {
    log->error() << "Invalid header for serialized memory location.";
    return false;
  }

  uint16_t numAllowed;
  if (!buffer.read(&numAllowed, sizeof(numAllowed))) {
    return false;
  }
  for (uint16_t i=0; i<numAllowed; i++) {
    MemoryLocationConstraint c(true);
    if (!c.deserialize(log, buffer)) {
      return false;
    }
    add(c);
  }
  
  uint16_t numDenied;
  if (!buffer.read(&numDenied, sizeof(numDenied))) {
    return false;
  }
  for (uint16_t i=0; i<numDenied; i++) {
    MemoryLocationConstraint c(false);
    if (!c.deserialize(log, buffer)) {
      return false;
    }
    add(c);
    return false;
  }

  return true;
}