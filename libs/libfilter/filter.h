#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using row = std::vector<unsigned short>;
using column = std::vector<row>;


auto split(const std::string &str) -> row;
    
void ip_fill(column &ip_pool);

void ip_print(const column &ip_pool);
    
void sort_lexicographical(column &ip_pool);

template<typename... Args>
auto filter(const column &ip_pool, Args... vals) -> column
{
    column retval;
    auto matches = {vals...};
    auto sz = matches.size();
    
    std::copy_if(ip_pool.begin(), ip_pool.end(), std::back_inserter(retval), [&matches, &sz](const auto &ip){
        return std::equal(ip.begin(), ip.begin() + sz, matches.begin());
    });   

    return retval;
}

template<typename... Args>
auto filter_any(const column &ip_pool, Args... vals) -> column
{
    column retval;
    auto matches = {vals...};
    
    std::copy_if(ip_pool.begin(), ip_pool.end(), std::back_inserter(retval), [&matches](const auto &ip){
        return std::any_of(ip.begin(), ip.end(), [&matches](const auto &ip_part){
            return std::find(matches.begin(), matches.end(), ip_part) != matches.end();
        });
    });
    
    return retval;
}