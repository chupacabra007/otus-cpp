#pragma once

#include <cstddef>
#include "FixedAllocator.h"

class SmallObjAllocator
{
public:
    SmallObjAllocator(size_t pageSize, size_t maxObjectSize);
    void* allocate(size_t numBytes);
    void deallocate(void* p, size_t numBytes);
private:
    FixedAllocator* pool;
    size_t maxObjectSize;
};