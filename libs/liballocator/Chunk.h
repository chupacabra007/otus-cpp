#pragma once

#include <cstddef>

using uchar = unsigned char;


class Chunk 
{
private:	

    friend class FixedAllocator;

    void init(size_t blockSize, uchar blocks);
    
    void release();
    
    void* allocate(size_t blockSize);
    
    void deallocate(void* p, size_t blockSize);
    
    inline bool hasBlock(void* p, size_t chunkLen) const
    {
        uchar * pc = static_cast<uchar*>(p);
        return (pData <= pc) && (pc <= (pData + chunkLen));
    }
    
    inline bool releasable(uchar numBlocks) const
    {
    	  return blocksAvailable == numBlocks;    
    }
    
    uchar* pData;
    
    uchar firstAvailableBlock, blocksAvailable;    
};