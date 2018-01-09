#pragma once

#include "node.h"
#include <memory>

template<typename T, typename Alloc = std::allocator<node<T>>>
class slist
{
private:
    node<T>* m_head = nullptr;
    node<T>* m_tail = nullptr;
    Alloc allocator;
public:

    class iterator 
    {
    private:
        node<T>* m_node; 
    public:
        iterator(node<T>* node): m_node(node) {};
        
        bool operator==(const iterator& other)
        {
            return this == &other || m_node == other.m_node;        
        }
        
        bool operator!=(const iterator& other)
        {
            return !operator==(other);        
        }
        
        T operator*()
        {       
            if (m_node)
            {
                return m_node->m_value;
            }
            return T();
        } 
        
        T* operator->()
        {
            if (m_node)
            {
                return m_node;            
            }        
            return nullptr;
        }
        
        iterator operator++() 
        {
            iterator i = *this;
            if (m_node) 
            {
                m_node = m_node->m_next;
            }
            return i;
        }       
          
    };
    

    template<typename... Args>
    void emplace(T v)    
    {
    	  auto new_node = allocator.allocate(1);   	  
    	  allocator.construct(new_node, v);   	  
    	  if (m_head)
    	  {
            m_tail->m_next = new_node;    	  
    	  } else {
            m_head = new_node;
            new_node->m_next = nullptr;    	  
    	  }
        m_tail = new_node;   	  
    }
    
    
    void remove()
    {
        if (m_head)
        {
            node<T>* new_head = m_head->m_next;  
            allocator.destroy(m_head);
    	      allocator.deallocate(m_head, 1);
    	      m_head = new_head;  
        }    
    }
    
    
    iterator begin() const
    {
        return iterator(m_head);
    }
    
    
    iterator end() const
    {
        return iterator(nullptr);
    }
    
    size_t size() const
    {
        size_t sz = 0;
        for (iterator it = begin(); it != end(); ++it)  
        {
            ++sz;        
        }
        return sz;
    }
};