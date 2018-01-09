#include "Chunk.h"

using uchar = unsigned char;

void Chunk::init(size_t blockSize, uchar blocks)
{
	 // for n of Ts it will allocate n * sizeof(T) memory
    pData = new uchar[blockSize * blocks];
    firstAvailableBlock = 0;
    blocksAvailable = blocks;
    uchar i = 0;
    uchar* p = pData;
    // used by allocate method to move forward firstAvailableBlock 
    for (; i != blocks; p += blockSize) 
    {
    	  *p = ++i;
    }
}


void Chunk::release()
{
    ::operator delete(pData);
}


void* Chunk::allocate(size_t blockSize)
{
	 if (!blocksAvailable) return 0;
	 // move firstAvailableBlock one block ahead
    uchar* pResult = pData + firstAvailableBlock * blockSize;
    firstAvailableBlock = *pResult;
    --blocksAvailable;
    return pResult;
}


void Chunk::deallocate(void* p, size_t blockSize)
{
    uchar* toRelease = static_cast<uchar*>(p);
    // find last but one available block
    firstAvailableBlock = static_cast<uchar>((toRelease - pData) / blockSize);
    ++blocksAvailable;
}