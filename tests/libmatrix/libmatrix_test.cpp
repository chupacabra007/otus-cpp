#define BOOST_TEST_MODULE libmatrix_test

#include <boost/test/included/unit_test.hpp>
#include "libmatrix/SpareMatrix.h"

BOOST_AUTO_TEST_SUITE(libmatrix_test)


BOOST_AUTO_TEST_CASE(test_libmatrix)
{
	 SpareMatrix<int, -1> m;
	 
	 BOOST_CHECK(m.size() == 0);
	 
	 m[200][200] = -1;
	 BOOST_CHECK(m.size() == 0);
	 
	 m[0][0] = 0;
	 m[1][1] = 1;
	 BOOST_CHECK(m.size() == 2);
}

BOOST_AUTO_TEST_SUITE_END()