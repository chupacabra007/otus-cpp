#define BOOST_TEST_MODULE libfilter_test

#include <boost/test/included/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include "libfilter/filter.h"

BOOST_AUTO_TEST_SUITE(libfilter_test)


BOOST_AUTO_TEST_CASE(test_sort_lexicographical)
{
	 column ip_pool;
	 
	 ip_pool.push_back({1,7,6,5});
	 ip_pool.push_back({1,3,8,9});
	 ip_pool.push_back({1,8,4,2});
	 ip_pool.push_back({2,1,1,3});
	 ip_pool.push_back({2,2,3,4});
	 
	 sort_lexicographical(ip_pool);
	 //2.2.3.4
	 //2.1.1.3
	 //1.8....
	 //1.7....
	 //1.3....
	 
	 row r0 = ip_pool[0];
	 BOOST_CHECK(r0[0] == 2 && r0[1] == 2);
	 
	 row r1 = ip_pool[1];
	 BOOST_CHECK(r1[0] == 2 && r1[1] == 1);
	 
	 row r2 = ip_pool[2];
	 BOOST_CHECK(r2[0] == 1 && r2[1] == 8);
	 
	 row r3 = ip_pool[3];
	 BOOST_CHECK(r3[0] == 1 && r3[1] == 7);
	 
	 row r4 = ip_pool[4];
	 BOOST_CHECK(r4[0] == 1 && r4[1] == 3);	 
	
}

BOOST_AUTO_TEST_CASE(test_filter_2)
{
	 column ip_pool;
	 
	 ip_pool.push_back({1,7,6,5});
	 ip_pool.push_back({1,3,8,9});
	 ip_pool.push_back({1,8,4,2});
	 ip_pool.push_back({2,1,1,3});
	 ip_pool.push_back({2,2,3,4});
	 
	 column nc = filter(ip_pool, 2);
	 //2.2.3.4
	 //2.1.1.3
	 
	 row r0 = nc[0];
	 row r1 = nc[1];
	 BOOST_CHECK(nc.size() == 2 && r0[0] == 2 && r1[0] == 2);
	 
	
}

BOOST_AUTO_TEST_CASE(test_filter_3_8)
{
	 column ip_pool;
	 
	 ip_pool.push_back({1,7,6,0});
	 ip_pool.push_back({1,3,8,9});
	 ip_pool.push_back({3,8,4,2});
	 ip_pool.push_back({2,5,6,3});
	 ip_pool.push_back({2,2,3,4});
	 
	 column nc = filter(ip_pool, 3, 8);
	 //3.8.4.2
	 
	 row r0 = nc[0];
		 
	 BOOST_CHECK(nc.size() == 1);
	 BOOST_CHECK(r0[0] == 3 && r0[1] == 8);
	
}

BOOST_AUTO_TEST_CASE(test_filter_any)
{
	 column ip_pool;
	 
	 ip_pool.push_back({2,7,6,5});
	 ip_pool.push_back({1,3,8,9});
	 ip_pool.push_back({1,8,4,2});
	 ip_pool.push_back({2,3,8,3});
	 ip_pool.push_back({2,2,3,1});
	 
	 column nc = filter_any(ip_pool, 1, 8);
	 	 
	 BOOST_CHECK(nc.size() == 4);
	
}

BOOST_AUTO_TEST_SUITE_END()