#pragma once

#include "UnboundArray.h"
#include "SpareArray.h"


template <typename T, const int EMPTY_ELEMENT>
class SpareArray;

template <typename T, const int EMPTY_ELEMENT>
class Proxy
{
private:
    SpareArray<T, EMPTY_ELEMENT> &self;
    std::size_t index;
public:
    Proxy(SpareArray<T, EMPTY_ELEMENT> &self, std::size_t index);
        
    T& operator=(const T &el);
    
    operator T&();
    
    UnboundArray<std::size_t> getCells();

    std::size_t size();
};


template <typename T, const int EMPTY_ELEMENT>
Proxy<T, EMPTY_ELEMENT>::Proxy(SpareArray<T, EMPTY_ELEMENT> &self, std::size_t index) : self(self), index(index) {}
  
        
template <typename T, const int EMPTY_ELEMENT>
T& Proxy<T, EMPTY_ELEMENT>::operator=(const T &el)
{
    if (el == EMPTY_ELEMENT)
    {
        if (self.indexExists(index)) 
        {
            std::size_t cell = self.cellIndex(index);
            self.remove(cell);
        }
        return *(new T(EMPTY_ELEMENT));   
    }        
    self.insert(index, el);
    return self.get(index);
}
    
    
template <typename T, const int EMPTY_ELEMENT>
Proxy<T, EMPTY_ELEMENT>::operator T&() 
{
    if (!self.indexExists(index))
    {
        return *(new T(EMPTY_ELEMENT));       
    }
    return self.get(index);
}
    
template <typename T, const int EMPTY_ELEMENT> 
UnboundArray<std::size_t> Proxy<T, EMPTY_ELEMENT>::getCells()
{
    return self.getCells();    
}


template <typename T, const int EMPTY_ELEMENT>
std::size_t Proxy<T, EMPTY_ELEMENT>::size()
{
    return self.size();    
}