#include <cstddef>
#include <iostream>
#include <vector>
#include <list>
#include <chrono>
#include <string>
using namespace std::chrono;

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
    return pResult;
}


void Chunk::deallocate(void* p, size_t blockSize)
{
    uchar* toRelease = static_cast<uchar*>(p);
    // find last but one available block
    firstAvailableBlock = static_cast<uchar>((toRelease - pData) / blockSize);
    ++blocksAvailable;
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
    void * allocate();
    void deallocate(void* p);
};


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


template<typename T, size_t numBlocks = 64>
class Allocator  
{
public:
    	
    Allocator(){};
                
    template<typename U, size_t N>
    Allocator(Allocator<U, N> const&);
    
    template<typename U>
    struct rebind 
    {
        using other = Allocator<U, numBlocks>;
    };
        
    T* allocate(size_t cnt) 
    {
    	  return reinterpret_cast<T*>(
    	      allocator.allocate(sizeof(T) * cnt)
    	  );       
    }
        
    void deallocate(T* p, size_t cnt) 
    {
        allocator.deallocate(p, sizeof(T) * cnt);
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
    
private:
    static SmallObjAllocator allocator;       
};


template<typename T, size_t numBlocks>
SmallObjAllocator Allocator<T, numBlocks>::allocator(numBlocks * sizeof(T), sizeof(T));

     
template<class List>
void test(std::string comment, List l)
{
    std::cout << comment;
    auto start_time = high_resolution_clock::now();
    for (int i = 0; i < 10000; ++i)
    {
        l.push_back(i);    
    }
    auto end_time = high_resolution_clock::now();
    std::cout << duration_cast<milliseconds>(end_time  - start_time).count() << "ms" << std::endl;
}


int main() {	 
	 test("default list ", std::list<int>());	
	 test("list with custom allocator ", std::list<int, Allocator<int, 10000>>());
	 return 0;
}