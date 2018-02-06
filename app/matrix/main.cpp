#include <iostream>
#include <vector> 
#include <cassert> 
#include <algorithm>
#include <utility>

/*
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

    UnboundArray<std::size_t> indices;
    
public:

    Proxy<T, EMPTY_ELEMENT> operator[](const int &index) 
    {
        return Proxy<T, EMPTY_ELEMENT>(*this, index);   
    }
    
    
    T& get(const int &index)
    {
         assert(index >= 0);
         std::size_t pos = indexOfValue(index);
         return UnboundArray<T>::get(pos);         
    }
    
    
    std::size_t indexOfValue(const int &index)
    {
        return indices.indexOf(index);
    }
    

    bool indexExists(const int &index)
    {
        auto it = find(indices.begin(), indices.end(), index);
        return it != indices.end();
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
            std::size_t pos = indexOfValue(index);
            UnboundArray<T>::remove(pos);
            UnboundArray<T>::insert(pos, el);
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
    
    
    void remove(const int &index)
    {
        UnboundArray<T>::remove(index);
        indices.remove(index);
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
                std::size_t pos = self.indexOfValue(index);
                self.remove(pos);
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
    
    
    auto begin()
    {
        return std::begin(values);    
    }
    
    
    auto end()
    {
        return std::end(values);    
    }
    
    
    auto begin() const
    {
        return std::cbegin(values);    
    }
    
    
    auto end() const
    {
        return std::cend(values);    
    }
      
    
};

*/


class Simple
{
private:
   std::vector<std::size_t> indices;
   std::vector<int> values;
   
public:
   
   void insert(std::size_t index, int value)
   {
       indices.push_back(index);
       values.push_back(value);
   }
   
   int at(std::size_t index)
   {
       return values[indices[index]];   
   }
   
   class Iterator
   {
   private:
       const std::vector<std::size_t>* indices;
       const std::vector<int>* values;
       std::size_t pos = 0;
   public:
       Iterator(const std::vector<std::size_t>* indices_, const std::vector<int>* values_, const std::size_t &pos_ = 0): 
           values(values_), indices(indices_), pos(pos_){ }
       
       bool operator==(const Iterator& other){
           return this == &other;       
       }
       
       bool operator!=(const Iterator& other){
           return !operator==(other);       
       }
       
       Iterator operator++() {
           pos++;
           Iterator i = *this;
           return i;       
       }
       
       std::pair<std::size_t, int> operator*()
       {
           if (pos < (*values).size())
           {
               return std::make_pair((*indices)[pos], (*values)[pos]);          
           }
           std::cout << "EMPTY PAIR" << std::endl; //loops infinitely
           return std::pair<std::size_t,int>{};           
       }
       
       std::pair<std::size_t, int>* operator->()
       {
           if (pos < (*values).size()) 
           {
               std::pair<std::size_t,int> *p;
               *p = std::make_pair((*indices)[pos], (*values)[pos]); 
               return p;       
           }  
           return nullptr;      
       }
   };
   
   Iterator begin() const
   {
       return Iterator(&indices, &values, 0);   
   }
   
   Iterator end() const
   {
       return Iterator(&indices, &values, values.size());   
   }
};

int main() {
    
    Simple s;
    s.insert(10, 100);
    std::cout << s.at(10) << std::endl;
    int i = 0;
    for (const std::pair<std::size_t, int> &p : s)
    {
        std::cout << p.first << " " << p.second << std::endl;   
        if (i > 3) break;
        i++; 
    }    
    /*
    SpareMatrix<int, 0> matrix;
    
    for (int i = 0; i < 10; ++i)
    {
        matrix[i][i] = i;    
    }    
    
    for (int i = 0; i < 10; ++i)
    {
        matrix[i][9 - i] = 9 - i;
    }
    
    
    for (int i = 1; i < 9; ++i)
    {
        for (int j = 1; j < 9; ++j)
        {
            std::cout << matrix[i][j] << " ";
        }  
        std::cout << std::endl;  
    }
        
    
    std::cout << std::endl;
    
    std::cout << "Number of cells occupied: " << matrix.size() << std::endl;
    
    std::cout << std::endl;
    
    for (const auto &row : matrix)
    {
        //std::cout << matrix.index(row) << std::endl;
    }   
    
    */
    
    return 0;
}