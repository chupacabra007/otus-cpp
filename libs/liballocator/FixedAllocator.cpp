#include "FixedAllocator.h"


FixedAllocator::FixedAllocator():
    blockSize(0),
    blocks(0),
    chunks(0),
    allocChunk(nullptr)
{
}


FixedAllocator::~FixedAllocator()
{
    Chunks::iterator it;
    for (it = chunks.begin(); it != chunks.end(); ++it)
    {
        it->release();    
    }
}


void FixedAllocator::init(size_t blockSize_, size_t pageSize)
{
	 blockSize = blockSize_;
    size_t numBlocks = pageSize / blockSize;
    blocks = static_cast<uchar>(numBlocks);
}


void* FixedAllocator::allocate()
{
	 if (!allocChunk || allocChunk->blocksAvailable == 0)
    {
        Chunks::iterator it = chunks.begin(); 
        int i = 0;   
        for (;;++it)
        {
        	   if (it == chunks.end())
            {
            	 // allocate memory for one more chunk
                chunks.reserve(chunks.size() + 1);
                Chunk newChunk;  
                newChunk.init(blockSize, blocks);
                // add new chunk to memory pool
                chunks.push_back(newChunk);                
                // points to new just initiated chunk
                allocChunk = &chunks.back();
                break;
            }
            if (it->blocksAvailable > 0)
            {
            	 // points to chunk with available blocks
                allocChunk = &*it;
                break;            
            }  
            ++i;                 
        }
    }
    return allocChunk->allocate(blockSize);
}


void FixedAllocator::deallocate(void* p)
{
    size_t chunkLen = blocks * blockSize;
    Chunks::iterator it;
    int cPos = 0;
    for (it = chunks.begin(); it != chunks.end(); ++it, ++cPos)
    {
        if (it->hasBlock(p, chunkLen))
        {
            it->deallocate(p, blockSize);  
            if (it->releasable(blocks)) {
                it->release();
                chunks.erase(chunks.begin() + cPos);
                // allocChunk may point to deleted chunk
                // so, reset it
                if (!chunks.empty()) {
                    allocChunk = &chunks.back();
                } else {
                    allocChunk = nullptr;                
                }
            } else {
                // there are free blocks in chunk
         	    // so, reset allocChunk for fast search
         	    allocChunk = &*it;    
            }
            break;   
        }    
    } 
}