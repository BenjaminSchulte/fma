#include "fma/Version.hpp"

// ----------------------------------------------------------------------------
const char *FMA::getLibraryVersion() {
# ifdef FMA_VERSION
    return FMA_VERSION;
# else
    return "?";
# endif
}

// ----------------------------------------------------------------------------