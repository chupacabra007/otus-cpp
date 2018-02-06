#pragma once

#include <cstddef>
#include <algorithm>
#include "UnboundArray.h"
#include "Proxy.h"

template <typename T, const int EMPTY_ELEMENT>
class Proxy;


template <typename T, const int EMPTY_ELEMENT>
class SpareArray : public UnboundArray<T> 
{
private:

    UnboundArray<std::size_t> cells;
    
public:

    Proxy<T, EMPTY_ELEMENT> operator[](const int &index);
    
    T& get(const int &index);    

    UnboundArray<std::size_t> getCells();
    
    std::size_t cellIndex(const int &index);    

    bool indexExists(const int &index);    
    
    void append(const T &el);
    
    void insert(const int &index, const T &el);    
    
    int maxInsertedIndex();    
    
    void remove(const int &index);    
    
    void rangeBoundCheck(const int &index); 
    
    void rangeUnboundCheck(const int &index);
};



template <typename T, const int EMPTY_ELEMENT>
Proxy<T, EMPTY_ELEMENT> SpareArray<T, EMPTY_ELEMENT>::operator[](const int &index) 
{
    return Proxy<T, EMPTY_ELEMENT>(*this, index);   
}
    

template <typename T, const int EMPTY_ELEMENT>    
T& SpareArray<T, EMPTY_ELEMENT>::get(const int &index)
{
     assert(index >= 0);
     std::size_t cell = cellIndex(index);
     return UnboundArray<T>::get(cell);         
}
    

template <typename T, const int EMPTY_ELEMENT>
UnboundArray<std::size_t> SpareArray<T, EMPTY_ELEMENT>::getCells()
{
    return cells;    
}


template <typename T, const int EMPTY_ELEMENT>   
std::size_t SpareArray<T, EMPTY_ELEMENT>::cellIndex(const int &index)
{
    return cells.indexOf(index);
}
    

template <typename T, const int EMPTY_ELEMENT>
bool SpareArray<T, EMPTY_ELEMENT>::indexExists(const int &index)
{
    auto it = find(cells.begin(), cells.end(), index);
    return it != cells.end();
}    
    

template <typename T, const int EMPTY_ELEMENT>   
void SpareArray<T, EMPTY_ELEMENT>::append(const T &el)
{
    UnboundArray<T>::append(el);
    cells.append(maxInsertedIndex() + 1);
}
    

template <typename T, const int EMPTY_ELEMENT>    
void SpareArray<T, EMPTY_ELEMENT>::insert(const int &index, const T &el)
{
    assert(index >= 0);        
    if (!indexExists(index)) 
    {
        UnboundArray<T>::append(el);
        cells.append(index);
    }
    else 
    {
        std::size_t cell = cellIndex(index);
        UnboundArray<T>::remove(cell);
        UnboundArray<T>::insert(cell, el);
    }
}
    

template <typename T, const int EMPTY_ELEMENT>   
int SpareArray<T, EMPTY_ELEMENT>::maxInsertedIndex()
{
    if (!cells.isEmpty())
    {
        return std::distance(cells.begin(), 
                             std::max_element(cells.begin(), cells.end()));
    }
    return -1;
}
    

template <typename T, const int EMPTY_ELEMENT>    
void SpareArray<T, EMPTY_ELEMENT>::remove(const int &index)
{
    UnboundArray<T>::remove(index);
    cells.remove(index);
}
    

template <typename T, const int EMPTY_ELEMENT>    
void SpareArray<T, EMPTY_ELEMENT>::rangeBoundCheck(const int &index) {}  


template <typename T, const int EMPTY_ELEMENT>   
void SpareArray<T, EMPTY_ELEMENT>::rangeUnboundCheck(const int &index) {}