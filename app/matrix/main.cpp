#include <iostream>
#include <vector> 
#include <cassert> 
#include <algorithm>



template <typename T>
class UnboundArray
{        
private:

    static const std::size_t DEFAULT_CAPACITY = 10;
    std::size_t mSize = 0;
    std::vector<T> elementData;
    
public:

    UnboundArray() : UnboundArray(DEFAULT_CAPACITY) {}
    

    UnboundArray(std::size_t initialCapacity) 
    {
        elementData.reserve(initialCapacity);
    }
    
    T& operator[] (const int &index)
    {
        rangeUnboundCheck(index);
        return elementData[index];
    }
    
    
    T const& operator[] (const int &index) const
    {
        rangeUnboundCheck(index);
        return elementData[index];
    }
    
    
    T& get(const int &index)
    {
        rangeUnboundCheck(index);
        return elementData[index];
    }
    
    
    void append(const T &el)
    {
        ensureCapacity(++mSize);
        elementData.push_back(el); 
    }
    
    
    void insert(const int &index, const T &el)
    {
        rangeBoundCheck(index);
        ensureCapacity(++mSize);
        elementData.insert(elementData.begin() + index, el); 
    }
    
    
    void remove(const int &index)
    {
        rangeUnboundCheck(index);
        elementData.erase(elementData.begin() + index); 
        --mSize;
    }
    
    
    void ensureCapacity(std::size_t minCapacity)
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
    
    
    void rangeBoundCheck(const int &index)
    {
        assert(index >= 0 && index <= mSize);
    }
    
    
    void rangeUnboundCheck(const int &index)
    {
        assert(index >= 0 && index < mSize);
    }
    
    
    bool isEmpty()
    {
        return mSize == 0;    
    }
    
    
    std::size_t size() const
    {
        return mSize;        
    }   


    auto begin()
    {
        return std::begin(elementData);    
    }
    
    
    auto end()
    {
        return std::end(elementData);    
    }
    
    
    auto begin() const
    {
        return std::cbegin(elementData);    
    }
    
    
    auto end() const
    {
        return std::cend(elementData);    
    }

    
};


template <typename T, const int EMPTY_ELEMENT>
class Proxy;


template <typename T, const int EMPTY_ELEMENT>
class SpareArray : public UnboundArray<T> 
{
private:

    UnboundArray<int> indices;
    
public:

    Proxy<T, EMPTY_ELEMENT> operator[](const int &index) 
    {
        return Proxy<T, EMPTY_ELEMENT>(*this, index);   
    }
    
    
    T& get(const int &index)
    {
         assert(index >= 0);
         std::size_t index_of_index = indexOf(index);
         return UnboundArray<T>::get(index_of_index);         
    }
    

    bool indexExists(const int &index)
    {
        auto it = find(indices.begin(), indices.end(), index);
        return it != indices.end();
    }    
    
    
    std::size_t indexOf(const int &index)
    {
        auto it = find(indices.begin(), indices.end(), index);
        if (it != indices.end()) {
            return distance(indices.begin(), it);
        }
        return -1; 
    }
    
        
    void append(const T &el)
    {
        UnboundArray<T>::append(el);
        indices.append(maxInsertedIndex() + 1);
    }
    
    
    void insert(const int &index, const T &el)
    {
        assert(index >= 0);        
        if (!indexExists(index)) 
        {
            UnboundArray<T>::append(el);
            indices.append(index);
        }
        else 
        {
            std::size_t index_of_index = indexOf(index);
            UnboundArray<T>::remove(index_of_index);
            UnboundArray<T>::insert(index_of_index, el);
        }
    }
    
    
    int maxInsertedIndex()
    {
        if (!indices.isEmpty())
        {
            return std::distance(indices.begin(), 
                                 std::max_element(indices.begin(), indices.end()));
        }
        return -1;
    }
    
    
    void rangeBoundCheck(const int &index) {}  
    
    void rangeUnboundCheck(const int &index) {}   
};


template <typename T, const int EMPTY_ELEMENT>
class Proxy
{
private:

    SpareArray<T, EMPTY_ELEMENT> &self;
    std::size_t index;
    
public:

    Proxy(SpareArray<T, EMPTY_ELEMENT> &self, std::size_t index) : self(self), index(index) {}
    
    
    T& operator=(const T &el)
    {
        if (el == EMPTY_ELEMENT)
        {
            if (self.indexExists(index)) 
            {
                std::size_t index_of_index = self.indexOf(index);
                self.remove(index_of_index);
            }
            return *(new T(EMPTY_ELEMENT));   
        }        
        self.insert(index, el);
        return self.get(index);
    }
    
    
    operator T&() 
    {
        if (!self.indexExists(index))
        {
            return *(new T(EMPTY_ELEMENT));       
        }
        return self.get(index);
    }
    

};



template <typename T, const int EMPTY_ELEMENT>
class SpareMatrix
{
private:

    SpareArray<SpareArray<T, EMPTY_ELEMENT>, EMPTY_ELEMENT> values;    

public:
    
    SpareArray<T, EMPTY_ELEMENT>& operator[](const int &index)
    {
        if (!values.indexExists(index))
        {
            SpareArray<T, EMPTY_ELEMENT> row;
            values.insert(index, row);
        }
        return values.get(index);   
    }  
    
    
    std::size_t size()
    {
        std:size_t sz = 0;
        for (const auto &row : values)
        {
            sz += row.size();        
        }
        return sz;
    }  
};



int main() {
    SpareMatrix<int, -1> matrix;
    assert(matrix.size() == 0);
    auto a = matrix[0][0];
    assert(a == -1);
    
    matrix[100][100] = 314;
    assert(matrix[100][100] == 314);
    assert(matrix.size() == 1);
    
    /*matrix[100][100] = 314;
    matrix[101][101] = -1;
    std::cout << matrix.size() << std::endl;
    
    SpareArray<int, -1> arr;
    arr[1] = 1;
    arr[100] = 100;
    std::cout << arr.size() << std::endl;
    std::cout << arr[0] << std::endl;
    std::cout << matrix[100][200] << std::endl;
    arr[100] = -1;
    arr[1] = -1;
    std::cout << arr.size() << std::endl;
    */
    return 0;
}