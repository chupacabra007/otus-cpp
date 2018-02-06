#pragma once

#include <cstddef>
#include <map>
#include <utility>
#include "SpareArray.h"


template <typename T, const int EMPTY_ELEMENT>
class SpareMatrix
{
private:

    SpareArray<SpareArray<T, EMPTY_ELEMENT>, EMPTY_ELEMENT> values;  

public:

    SpareArray<T, EMPTY_ELEMENT>& operator[](const int &index);    
    
    std::size_t size();    
    
    const std::multimap<std::size_t, std::size_t> getXY();    

};


template <typename T, const int EMPTY_ELEMENT>
SpareArray<T, EMPTY_ELEMENT>& SpareMatrix<T, EMPTY_ELEMENT>::operator[](const int &index)
{
    if (!values.indexExists(index))
    {
        SpareArray<T, EMPTY_ELEMENT> row;
        values.insert(index, row);
    }
    return values.get(index);   
}  
    

template <typename T, const int EMPTY_ELEMENT> 
std::size_t SpareMatrix<T, EMPTY_ELEMENT>::size()
{
    std:size_t sz = 0;
    for (const auto &row : values)
    {
        sz += row.size();        
    }
    return sz;
}
    
template <typename T, const int EMPTY_ELEMENT>     
const std::multimap<std::size_t, std::size_t> SpareMatrix<T, EMPTY_ELEMENT>::getXY()
{
    std::multimap<std::size_t,std::size_t> xy;
    for (const auto &y : values.getCells())
    {           
        for (const auto &x : (*this)[y].getCells())
        {
            if ((*this)[y][x] != EMPTY_ELEMENT)
            {
                xy.insert(std::pair<std::size_t,std::size_t>(x, y));
            }
        }   
    }
    return xy;  
}