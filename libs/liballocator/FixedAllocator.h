#pragma once

#include <cstddef>
#include <vector>
#include "Chunk.h"

using uchar = unsigned char;


class FixedAllocator 
{
private:
    size_t blockSize;
    uchar blocks;
    using Chunks = std::vector<Chunk>;
    Chunks chunks;
    Chunk* allocChunk;
public:
    FixedAllocator();
    ~FixedAllocator();
    void init(size_t blockSize, size_t pageSize);
    void * allocate();
    void deallocate(void* p);
};