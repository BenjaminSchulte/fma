#include <boost/io/ios_state.hpp>
#include <memory/MemoryMapBank.hpp>
#include <memory/MemoryMapBankSection.hpp>
#include <memory/MemoryMapShadow.hpp>
#include <iostream>
#include <iomanip>

using namespace FMA::memory;

// ----------------------------------------------------------------------------
MemoryMapBankSection::MemoryMapBankSection(MemoryBankSize offset, MemoryBankSize size)
  : offset(offset)
  , size(size)
  , shadow(NULL)
  , useLocation(false)
  , location(0)
{
}

// ----------------------------------------------------------------------------
MemoryMapBankSection::~MemoryMapBankSection()
{
}

// ----------------------------------------------------------------------------
void MemoryMapBankSection::dump(MemoryMapBank *bank, bool first) {
  boost::io::ios_all_saver guard(std::cout);

  std::cout << "| $" << std::setw(2) << std::setfill('0') << std::hex << std::uppercase;
  if (first) {
    std::cout << (int)bank->getIndex();
  } else {
    std::cout << "  ";
  }
  std::cout << " | $" << std::setw(4) << offset << "-$" << std::setw(4) <<(offset + size - 1) << " | ";
  
  if (useLocation) {
    std::cout << "$" << std::setw(6) << location << "-$" << std::setw(6) << (location + size - 1) << "  | ";
  } else {
    std::cout << "(No ROM mapping) | ";
  }

  if (shadow) {
    std::cout << "$" << std::setw(2) << (int)shadow->getOtherBank(bank)->getIndex() << ": $";
    std::cout << std::setw(4) << shadow->getOtherAddress(bank) << "-$" << std::setw(4) << (shadow->getOtherAddress(bank) + size - 1) << " |";
  } else {
    std::cout << "                 |";
  }

  std::cout << std::endl;

}

// ----------------------------------------------------------------------------
