#define BOOST_TEST_MODULE test_threadsafe_queue
#include <boost/test/unit_test.hpp>
#include "../include/threadsafe_queue.hpp"
#include <queue>

BOOST_AUTO_TEST_SUITE(test_threadsafe_queue)

BOOST_AUTO_TEST_CASE(test_push_and_pop)
{
    tinyInfra::threadsafe_queue<int> safe_q;
    std::queue<int> q;
    safe_q.push(10);
    q.push(10);

    BOOST_CHECK_EQUAL( safe_q.size(), q.size() );
    int value;
    if (safe_q.try_pop(value))
    {
        BOOST_CHECK_EQUAL( value, q.front() );
    }

    q.pop();
    BOOST_CHECK_EQUAL( safe_q.size(), q.size() );
}


BOOST_AUTO_TEST_SUITE_END()


