#include "SmallObjAllocator.h"

SmallObjAllocator::SmallObjAllocator(size_t pageSize, size_t maxObjectSize_):
    pool(nullptr),
    maxObjectSize(maxObjectSize_)
{
    pool = new FixedAllocator[maxObjectSize];
    for (size_t i = 0; i < maxObjectSize; ++i)
    {
    	  pool[i].init(i + 1, pageSize); 
    }
}


void* SmallObjAllocator::allocate(size_t numBytes) {
    if (numBytes > maxObjectSize)
    {
        return ::operator new(numBytes);    
    }    
    FixedAllocator& alloc = pool[numBytes-1];
    return alloc.allocate();
}


void SmallObjAllocator::deallocate(void* p, size_t numBytes)
{
    if (numBytes > maxObjectSize)
    {
        ::operator delete(p);   
        return; 
    }
    FixedAllocator& alloc = pool[numBytes-1];
    alloc.deallocate(p);
}