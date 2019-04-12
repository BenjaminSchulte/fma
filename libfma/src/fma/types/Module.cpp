#include <fma/types/Module.hpp>
#include <sstream>
#include <iostream>

using namespace FMA;
using namespace FMA::types;

// ----------------------------------------------------------------------------
Module::Module(const std::string &name)
  : name(name)
{
}

// ----------------------------------------------------------------------------
void Module::dump(const std::string &prefix) {
  std::cout << prefix << "module " << name << std::endl;

  for (auto const &member : members) {
    if (member.second->isModule()) {
      member.second->dump(prefix + "  ");
    } else if (member.second->isClass()) {
      member.second->dump(prefix + "  ");
    } else {
      std::cout << prefix << "  " << member.first << " = " << member.second->asString() << std::endl;
    }
  }

  std::cout << prefix << "end" << std::endl;
}

// ----------------------------------------------------------------------------
std::string Module::asString() const {
  std::ostringstream os;
  os << "<Module " << name << ">";
  return os.str();
}

// ----------------------------------------------------------------------------

