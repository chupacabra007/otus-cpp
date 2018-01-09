#pragma once

#include <cstddef>
#include "SmallObjAllocator.h"

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