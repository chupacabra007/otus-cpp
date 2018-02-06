#pragma once

#include <cstddef>
#include <vector>
#include <cassert>

template <typename T>
class UnboundArray
{        
private:

    static const std::size_t DEFAULT_CAPACITY = 10;
    std::size_t mSize = 0;
    std::vector<T> elementData;
    
public:

    UnboundArray();
    
    UnboundArray(std::size_t initialCapacity); 
    
    bool operator==(const UnboundArray& other);
    
    T& operator[] (const int &index);    
    
    T const& operator[] (const int &index) const;    
    
    T& get(const int &index);    
    
    void append(const T &el);    
    
    void insert(const int &index, const T &el);    
    
    void remove(const int &index);    
    
    void ensureCapacity(std::size_t minCapacity);
    
    void rangeBoundCheck(const int &index);
    
    void rangeUnboundCheck(const int &index);
    
    bool isEmpty();
    
    std::size_t indexOf(const T &el);
    
    std::size_t size() const;

    auto begin() const;
        
    auto end() const;
    
};



template <typename T>
UnboundArray<T>::UnboundArray() : UnboundArray(DEFAULT_CAPACITY) {}

   
template <typename T>   
UnboundArray<T>::UnboundArray(std::size_t initialCapacity) 
{
    elementData.reserve(initialCapacity);
}

 
template <typename T> 
bool UnboundArray<T>::operator==(const UnboundArray& other)
{
    return this == &other;
}

    
template <typename T>    
T& UnboundArray<T>::operator[] (const int &index)
{
    rangeUnboundCheck(index);
    return elementData[index];
}

    
template <typename T>   
T const& UnboundArray<T>::operator[] (const int &index) const
{
    rangeUnboundCheck(index);
    return elementData[index];
}

    
template <typename T>    
T& UnboundArray<T>::get(const int &index)
{
    rangeUnboundCheck(index);
    return elementData[index];
}

    
template <typename T>    
void UnboundArray<T>::append(const T &el)
{
    ensureCapacity(++mSize);
    elementData.push_back(el); 
}

    
template <typename T>    
void UnboundArray<T>::insert(const int &index, const T &el)
{
    rangeBoundCheck(index);
    ensureCapacity(++mSize);
    elementData.insert(elementData.begin() + index, el); 
}

   
template <typename T>   
void UnboundArray<T>::remove(const int &index)
{
    rangeUnboundCheck(index);
    elementData.erase(elementData.begin() + index); 
    --mSize;
}

    
template <typename T>    
void UnboundArray<T>::ensureCapacity(std::size_t minCapacity)
{
    std::size_t oldCapacity = elementData.capacity();
        
    if (minCapacity > oldCapacity) 
    {
        std::size_t newCapacity = (oldCapacity * 3 / 2) + 1;
        if (newCapacity < minCapacity)
            newCapacity = minCapacity;             
        elementData.reserve(newCapacity);
    }
}

    
template <typename T>   
void UnboundArray<T>::rangeBoundCheck(const int &index)
{
    assert(index >= 0 && index <= mSize);
}

    
template <typename T>    
void UnboundArray<T>::rangeUnboundCheck(const int &index)
{
    assert(index >= 0 && index < mSize);
}


template <typename T>    
bool UnboundArray<T>::isEmpty()
{
    return mSize == 0;    
}


template <typename T>    
std::size_t UnboundArray<T>::indexOf(const T &el)
{
    auto it = find(elementData.begin(), elementData.end(), el);
    if (it != elementData.end()) {
        return distance(elementData.begin(), it);
    }
    return -1; 
}
 
  
template <typename T>   
std::size_t UnboundArray<T>::size() const
{
    return mSize;        
}   


template <typename T>
auto UnboundArray<T>::begin() const
{
    return std::begin(elementData);    
}
 
 
template <typename T>   
auto UnboundArray<T>::end() const
{
    return std::end(elementData);    
}