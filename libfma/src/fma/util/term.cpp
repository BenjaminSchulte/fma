#include <fma/util/term.hpp>
#include <sstream>

using namespace FMA::util;

// ----------------------------------------------------------------------------
std::string FMA::util::setcolor(Color color, bool bright) {
# ifdef WINDOWS
    return "";
# else
    std::ostringstream os;
    os << "\x1b[" << std::hex << (color | 0x30) << (bright ? ";1" : "") << "m";
    return os.str();
# endif
}

// ----------------------------------------------------------------------------
std::string FMA::util::nocolor() {
# ifdef WINDOWS
    return "";
# else
    return "\x1b[m";
# endif 
}

