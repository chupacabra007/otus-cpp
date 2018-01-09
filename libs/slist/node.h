#pragma once

template<typename T> 
struct node
{
    node(): m_next(nullptr){}
    node(T const &t): m_value(t), m_next(nullptr){}
    T m_value;
    node* m_next;
};