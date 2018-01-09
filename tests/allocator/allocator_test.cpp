#define BOOST_TEST_MODULE allocator_test

#include <boost/test/included/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include "slist/slist.h"
#include "liballocator/Allocator.h"

BOOST_AUTO_TEST_SUITE(allocator_test)


BOOST_AUTO_TEST_CASE(test_slist_allocator)
{
	 auto s = slist<int,Allocator<node<int>,10>>{};
	 for(int i = 0; i < 10; ++i)
	 {
        s.emplace(i);	 
	 }
	 
	 BOOST_CHECK(s.size() == 10);	
}

BOOST_AUTO_TEST_SUITE_END()