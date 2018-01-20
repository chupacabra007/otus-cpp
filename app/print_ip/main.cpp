#include <iostream>
#include <vector>
#include <list>
#include <string>


template<typename T>
struct is_string : public std::integral_constant<bool, std::is_same<char*, typename std::decay<T>::type>::value || std::is_same<const char*, typename std::decay<T>::type>::value || std::is_same<std::string, typename std::decay<T>::type>::value> {};

template <typename T>
struct is_container { static const bool value = false; };

template <typename T>
struct is_container<std::vector<T>> { static const bool value = true; };

template <typename T>
struct is_container<std::list<T>> { static const bool value = true; };

template<typename T, std::enable_if_t<is_string<T>::value, int> = 0>
void print_ip(T t) {
    std::cout << t << std::endl;
}

template<typename T, std::enable_if_t<std::is_integral<T>::value, int> = 1>
void print_ip(T t) {
    int offset = (sizeof(T) - 1) * 8;
    while (offset > 0)
    {
        std::cout << ((t >> offset) & 0xFF) << ".";
        offset -= 8;    
    }
    std::cout << (t & 0xFF) << std::endl;
}

template<typename T, std::enable_if_t<is_container<T>::value, int> = 2>
void print_ip(T t) {
    bool flag = false;
    for (const auto &ip_part : t)
    {
        if (flag) std::cout << ".";	     	   
        std::cout << ip_part;
        flag = true;	         
	}
    std::cout << std::endl;
}


int main()
{
    char v1 = -1;
    print_ip(v1);
    
    short v2 = 0;
    print_ip(v2);
    
    int v3 = 2130706433;
    print_ip(v3);
    
    long v4 = 8875824491850138409;
    print_ip(v4);
    
    std::string v5 = "192.168.1.100";
    print_ip(v5);
    
    std::list<int> v6{192,168,1,200};
    print_ip(v6);
    
    std::vector<int> v7{192,168,1,201};
    print_ip(v7);
    
    return 0;
}