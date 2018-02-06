#include <iostream>
#include <vector> 
#include <map> 
#include <cassert> 
#include <algorithm>
#include <utility>


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
    
    
    bool operator==(const UnboundArray& other)
    {
        return this == &other;
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
    
    std::size_t indexOf(const T &el)
    {
        auto it = find(elementData.begin(), elementData.end(), el);
        if (it != elementData.end()) {
            return distance(elementData.begin(), it);
        }
        return -1; 
    }
    
    
    std::size_t size() const
    {
        return mSize;        
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

    UnboundArray<std::size_t> cells;
    
public:

    Proxy<T, EMPTY_ELEMENT> operator[](const int &index) 
    {
        return Proxy<T, EMPTY_ELEMENT>(*this, index);   
    }
    
    
    T& get(const int &index)
    {
         assert(index >= 0);
         std::size_t cell = cellIndex(index);
         return UnboundArray<T>::get(cell);         
    }
    

    UnboundArray<std::size_t> getCells()
    {
        return cells;    
    }

    
    std::size_t cellIndex(const int &index)
    {
        return cells.indexOf(index);
    }
    

    bool indexExists(const int &index)
    {
        auto it = find(cells.begin(), cells.end(), index);
        return it != cells.end();
    }    
    
    
    void append(const T &el)
    {
        UnboundArray<T>::append(el);
        cells.append(maxInsertedIndex() + 1);
    }
    
    
    void insert(const int &index, const T &el)
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
    
    
    int maxInsertedIndex()
    {
        if (!cells.isEmpty())
        {
            return std::distance(cells.begin(), 
                                 std::max_element(cells.begin(), cells.end()));
        }
        return -1;
    }
    
    
    void remove(const int &index)
    {
        UnboundArray<T>::remove(index);
        cells.remove(index);
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
                std::size_t cell = self.cellIndex(index);
                self.remove(cell);
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
    
    
    UnboundArray<std::size_t> getCells()
    {
        return self.getCells();    
    }

    std::size_t size()
    {
        return self.size();    
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
    
    
    const std::multimap<std::size_t, std::size_t> getXY()
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
    

};



int main() {
    
    
    SpareMatrix<int, 0> matrix;
    
    for (int i = 0; i < 10; ++i)
    {
        matrix[i][i] = i;    
    }    
    
    for (int i = 0; i < 10; ++i)
    {
        matrix[i][9 - i] = 9 - i;
    }
    
    std::cout << std::endl;
    
    for (int i = 1; i < 9; ++i)
    {
        for (int j = 1; j < 9; ++j)
        {
            std::cout << matrix[i][j] << " ";
        }  
        std::cout << std::endl;  
    }
    
    matrix[100][900] = 999;
    matrix[901][801] = 777;
    
    std::cout << std::endl;
    
    std::cout << "Number of cells occupied: " << matrix.size() << std::endl;
    
    std::cout << std::endl;
    
    std::cout << "Cells: " << std::endl;
    std::cout << "r c v" << std::endl;
    
    for (const auto &cell : matrix.getXY())
    {
         std::cout << cell.second << " " << cell.first << " " 
                   << matrix[cell.second][cell.first] << std::endl;
    }
 
    
    return 0;
}