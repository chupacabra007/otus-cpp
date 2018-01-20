#include <iostream>
#include <map>
#include "liballocator/Allocator.h"
#include "slist/slist.h"

#define maxn 100000

long long memo[maxn];


long long factorial(int n)
{
	if (n < 2) return 1;
	if (memo[n]) return memo[n];
    long long res = 1;
    for (int i = 2; i <= n; ++i)
    {
        res *= i;    
    }
    return memo[n] = res;
}

int main() {	 
	 auto m1 = std::map<int,int>{};
	 for(int i = 0; i < 10; ++i)
	 {
	 	  m1.insert(std::pair<int,int>(i,factorial(i)));
	 }	 
	 
	 auto m2 = std::map<int,int, std::less<int>, Allocator<std::pair<const int, int>, 10>>{};
	 for(int i = 0; i < 10; ++i)
	 {
	 	  m2.insert(std::pair<int,int>(i,factorial(i)));
	 }
	 
	 for (auto const &item : m2)
	 {
         std::cout << item.first << " " << item.second << std::endl;
	 }
	 	 
	 auto c1 = slist<int>{};	 
	 for(int i = 0; i < 10; ++i)
	 {
         c1.emplace(i);	 
	 }
	 
	 
	 auto c2 = slist<int,Allocator<int,10>>{};	 
	 for(int i = 0; i < 10; ++i)
	 {
         c2.emplace(i);	 
	 }
	 	 
	 for(auto it : c2)
	 {
	     std::cout << it << std::endl;
	 }	 
	 
	 return 0;
}