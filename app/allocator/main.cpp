#include <cstddef>
#include <iostream>
#include <vector>

// much credit to Alexandrescu and Loki library

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
    std::cout << "blocksAvailable after allocate " << blocksAvailable << std::endl;
    return pResult;
}

void Chunk::deallocate(void* p, size_t blockSize)
{
    uchar* toRelease = static_cast<uchar*>(p);
    // find last but one available block
    firstAvailableBlock = static_cast<uchar>((toRelease - pData) / blockSize);
    ++blocksAvailable;
    std::cout << "blocksAvailable after deallocate " << blocksAvailable << std::endl;
}

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
    const int blockOwner(void* p) const;
    void * allocate();
    void deallocate(void* p);
};

FixedAllocator::FixedAllocator()
    :blockSize(0),
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

const int FixedAllocator::blockOwner(void* p) const
{
    size_t chunkLen = blocks * blockSize;
    std::vector<int>::size_type i = 0, sz = chunks.size();
    for (; i < sz; i++)
    {
    	  if (chunks[i].hasBlock(p, chunkLen))
        {
        	   return i;        
        }    
    }
    return -1;
}

void* FixedAllocator::allocate()
{
	 if (!allocChunk || allocChunk->blocksAvailable == 0)
    {
        Chunks::iterator i = chunks.begin();    
        for (;;++i)
        {
            if (i == chunks.end())
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
            if (i->blocksAvailable > 0)
            {
            	 // points to chunk with available blocks
                allocChunk = &*i;
                break;            
            }                   
        }
    }
    return allocChunk->allocate(blockSize);
}

void FixedAllocator::deallocate(void* p)
{
    // TODO. Optimize. Now very bruteforce and non-efficient
    const int chunkPos = blockOwner(p); 
    if (chunkPos != -1) 
    {
    	   Chunk chunk = chunks[chunkPos];
    	   chunk.deallocate(p, blockSize);         
         // if chunk is releasable, release memory 
         if (chunk.releasable(blocks))
         {
             chunk.release();
             chunks.erase(chunks.begin() + chunkPos);
             // allocChunk may point to deleted chunk
             // so, reset it                      
             allocChunk = &chunks.back();
         } else {
         	 // there are free blocks in chunk
         	 // so, reset allocChunk for faster future allocation
             allocChunk = &chunk;    
         }         
    }    
}

template<class T, size_t T_num_els>
struct Allocator  
{
    	
    Allocator(){};
                
    template<class U, size_t U_num_els>
    Allocator(Allocator<U, U_num_els> const&);
    
    template<class U>
    struct rebind 
    {
        using other = Allocator<U, T_num_els>;
    };
        
    T* allocate(size_t s) 
    {
    	  return static_cast<T*>(::operator new(s * sizeof(T)));        
    }
        
    void deallocate(T* p, size_t s) 
    {
        ::operator delete(p);
    }
        
    void construct(T* p, T const& val) 
    {
        ::new((void *)p) T(val);         
    } 
        
    void destroy(T* p) 
    {
        return ((T*) p)->~T();        
    } 
        
    using value_type = T;
       
};
    

int main() {
	 FixedAllocator* alloc = new FixedAllocator();
	 alloc->init(4, 12);
	 void* p = alloc->allocate();
	 void* q = alloc->allocate();
	 void* r = alloc->allocate();
	 alloc->deallocate(p);
	 alloc->deallocate(q);
	 alloc->deallocate(r);	 
	 return 0;
}