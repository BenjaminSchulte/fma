#include <memory/MemoryMapBank.hpp>
#include <memory/MemoryMapShadow.hpp>

using namespace FMA::memory;

// ----------------------------------------------------------------------------
MemoryMapShadow::MemoryMapShadow(MemoryMapBank *left, MemoryMapBank *right, MemoryBankSize leftAddress, MemoryBankSize rightAddress, MemoryBankSize size)
  : left(left)
  , right(right)
  , leftAddress(leftAddress)
  , rightAddress(rightAddress)
  , size(size)
{
}

// ----------------------------------------------------------------------------
MemoryMapShadow::~MemoryMapShadow() {
}

// ----------------------------------------------------------------------------
MemoryMapBank *MemoryMapShadow::getOtherBank(const MemoryMapBank *bank) {
  return left == bank ? right : left;
}

// ----------------------------------------------------------------------------
MemoryBankSize MemoryMapShadow::getOwnAddress(const MemoryMapBank *bank) {
  return left == bank ? leftAddress : rightAddress;
}

// ----------------------------------------------------------------------------
MemoryBankSize MemoryMapShadow::getOtherAddress(const MemoryMapBank *bank) {
  return left == bank ? rightAddress : leftAddress;
}

// ----------------------------------------------------------------------------
