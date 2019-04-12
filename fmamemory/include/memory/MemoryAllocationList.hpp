#ifndef __FMA_MEMORY_MEMORYALLOCATIONLIST_H__
#define __FMA_MEMORY_MEMORYALLOCATIONLIST_H__

#include <vector>
#include "./MemoryAllocation.hpp"

namespace FMA {
namespace memory {

class MemoryAllocationList : public std::vector<MemoryAllocationPtr> {  
public:
};

}
}

#endif
