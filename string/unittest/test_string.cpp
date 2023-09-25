#define BOOST_TEST_MODULE test_string
#include <boost/test/unit_test.hpp>
#include "../include/string.hpp"

BOOST_AUTO_TEST_SUITE(test_string)

BOOST_AUTO_TEST_CASE(test_constructor)
{
    BOOST_CHECK_EQUAL( tinyInfra::string().size() == std::string().size() );
    BOOST_CHECK_EQUAL( true == tinyInfra::string().empty() );
    BOOST_CHECK_EQUAL( tinyInfra::string("123").size() == std::string("123").size() );
    BOOST_CHECK_EQUAL( false == tinyInfra::string("123").empty() );
    BOOST_CHECK_EQUAL( tinyInfra::string("hello world").length() == std::string("hello world").length());
    BOOST_CHECK_EQUAL( 11 == std::string("hello world").length() );
}


BOOST_AUTO_TEST_SUITE_END()


