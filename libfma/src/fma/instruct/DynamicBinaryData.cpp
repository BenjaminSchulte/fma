#include <fma/instruct/DynamicBinaryData.hpp>
#include <sstream>
#include <cstring>

using namespace FMA;
using namespace FMA::assem;
using namespace FMA::instruct;

// ----------------------------------------------------------------------------
std::string DynamicBinaryData::getName() {
  return "REFERENCE";
}

// ----------------------------------------------------------------------------
std::string DynamicBinaryData::asString() {
  std::ostringstream os;
  os << "REFERENCE " << data->asString();
  return os.str();
}

// ----------------------------------------------------------------------------
