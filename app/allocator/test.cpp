#include <cstddef>
#include <iostream>
#include <vector>

using uchar = unsigned char;

class Chunk 
{
private:
    friend class FixedAllocator;
    void init(size_t blockSize, uchar blocks);
    void allocate(size_t blockSize);
    void deallocate(size_t blockSize);
    
    uchar* pData;
    
    uchar firstAvailableBlock, blocksAvailable;
};

void Chunk::init(size_t blockSize, uchar blocks)
{
	 blocksAvailable = blocks;
}

void Chunk::allocate(size_t blockSize)
{
	 --blocksAvailable;
}

void Chunk::deallocate(size_t blockSize)
{
    ++blocksAvailable;
}

class FixedAllocator
{
public:
    void init()
    {
        
    }
};

int main()
{
	 
    return 0;
}