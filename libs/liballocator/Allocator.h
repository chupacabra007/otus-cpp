#pragma once

#include <cstddef>
#include "SmallObjAllocator.h"

template<typename T, size_t num_of_blocks = 64>
class Allocator  
{
public:
    	
    Allocator() {};
                
    template<typename U, size_t N>
    Allocator(Allocator<U, N> const&) noexcept {}
    
    template<typename U>
    struct rebind 
    {
        using other = Allocator<U, num_of_blocks>;
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
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    
private:
    static SmallObjAllocator allocator;       
};


template<typename T, size_t num_of_blocks>
SmallObjAllocator Allocator<T, num_of_blocks>::allocator(num_of_blocks * sizeof(T), sizeof(T));

