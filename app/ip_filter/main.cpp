#include "libfilter/filter.h"

int main() {
	
    column ip_pool;
    ip_fill(ip_pool);
    
    sort_lexicographical(ip_pool);
    
    ip_print(ip_pool);
    
    ip_print(filter(ip_pool, 1));
    
    ip_print(filter(ip_pool, 46, 70));
    
    ip_print(filter_any(ip_pool, 46));
    
    return 0;
}