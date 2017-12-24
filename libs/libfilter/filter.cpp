#include "filter.h"


auto split(const std::string &str) -> row
{
    row r;
    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of('.');
    int count = 0;
    while(stop != std::string::npos)
    {
        r.push_back(std::stoi(str.substr(start, stop - start)));
        start = stop + 1;
        stop = str.find_first_of('.', start);
        if (++count == 5)
        {
            break;        
        }
    }

    r.push_back(std::stoi(str.substr(start)));

    return r;
}
    
    
void ip_fill(column &ip_pool)
{
    for(std::string line; std::getline(std::cin, line);)
    {
        ip_pool.push_back(split(line));
    }
}

void ip_print(const column &ip_pool)
{
	 for (const auto &ip : ip_pool)
	 {
	 	  bool flag = false;
	     for (const auto &ip_part : ip)
	     {
	     	   if (flag)
	     	   {
                std::cout << ".";	     	   
	     	   }
	     	   std::cout << ip_part;
	     	   flag = true;	         
	     }
	     std::cout << std::endl;
	 }	 
}
    
    
void sort_lexicographical(column &ip_pool) 
{
    std::sort(ip_pool.begin(), ip_pool.end(), [](const auto &ip1, const auto &ip2){
       return !std::lexicographical_compare(ip1.begin(), ip1.end(), ip2.begin(), ip2.end());
    });    	  	
}
    
