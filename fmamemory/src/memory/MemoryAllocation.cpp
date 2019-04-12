#include <boost/io/ios_state.hpp>
#include <memory/MemoryAllocation.hpp>
#include <memory/MemorySymbolMap.hpp>
#include <memory/MemoryClassMembers.hpp>
#include <fma/util/term.hpp>
#include <iostream>
#include <iomanip>

using namespace FMA::memory;
using namespace FMA::util;
using namespace FMA::interpret;

// ----------------------------------------------------------------------------
MemoryAllocation::MemoryAllocation(MemoryMap *map)
  : map(map)
  , _placement(new MemoryPlacement(map))
  , hasValidSize(false)
  , hasArraySize(false)
  , arraySize(1)
  , hasCachedSize(false)
  , hasSortedMembers(false)
{
}

//-----------------------------------------------------------------------------
MemoryAllocation::~MemoryAllocation() {
  delete _placement;
}

// ----------------------------------------------------------------------------
MemoryBankSize MemoryAllocation::getItemSize() {
  if (klass) {
    return MemoryClassMembers::getClassMembers(klass)->getSize();
  } else {
    return size;
  }
}

// ----------------------------------------------------------------------------
MemoryBankSize MemoryAllocation::getSize() {
  if (hasCachedSize) {
    return cachedSize;
  }

  cachedSize = getItemSize() * arraySize;

  hasCachedSize = true;
  return cachedSize;
}

// ----------------------------------------------------------------------------
void MemoryAllocation::addParent(const WeakMemoryAllocationPtr &parent) {
  parentScopes.push_back(parent);
}

// ----------------------------------------------------------------------------
void MemoryAllocation::setSize(const MemoryBankSize &_size) {
  hasValidSize = true;
  size = _size;
  hasCachedSize = false;
}

// ----------------------------------------------------------------------------
void MemoryAllocation::setClass(const types::ClassPtr &_klass) {
  hasValidSize = true;
  klass = _klass;
  hasSortedMembers = true;
  hasCachedSize = false;
}

// ----------------------------------------------------------------------------
void MemoryAllocation::setArraySize(const MemoryBankSize &size) {
  hasArraySize = true;
  arraySize = size;
  hasCachedSize = false;
}

// ----------------------------------------------------------------------------
void MemoryAllocation::dump(const std::string &prefix) {
  dumpRow(prefix);
}

// ----------------------------------------------------------------------------
void MemoryAllocation::writeSymbolReferences(MemorySymbolMap *symbols) {
  if (!_placement->isValid() || !hasReference()) {
    return;
  }

  symbols->resolve(reference, *_placement);
}

// ----------------------------------------------------------------------------
void MemoryAllocation::dumpRowNoLocation(const std::string &prefix) {
  boost::io::ios_all_saver guard(std::cout);

  if (!requiresPlacement()) {
    std::cout << "   ";
  } else if (!_placement->isValid()) {
    std::cout << color::redBright() << "XX " << color::off();
  } else {
    std::cout << color::greenBright() << "OK " << color::off();
  }

  std::cout << prefix << color::pink() << name << color::off();
  if (_placement->isValid()) {
    std::cout << color::pinkBright() << "@($" << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << (int)_placement->getBank() << ":";
    std::cout << "$" << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << _placement->getOffset() << ")" << color::off();
  }
  if (hasValidSize) {
    std::cout << color::yellow() << " (SIZE:$" << std::hex << std::uppercase << getSize() << std::nouppercase << std::dec << ")" << color::off();
  } else {
    std::cout << color::blue() << " (SIZE:?)" << color::off();
  }
}

// ----------------------------------------------------------------------------
void MemoryAllocation::dumpRow(const std::string &prefix) {
  dumpRowNoLocation(prefix);

  for (const auto &allow : _locations.getAllowed()) {
    std::cout << color::green() << " +" << allow.asString() << color::off();
  }
  for (const auto &deny : _locations.getDenied()) {
    std::cout << color::red() << " -" << deny.asString() << color::off();
  }

  std::cout << std::endl;
}
