// main() provided by Catch in test_main.cc

#include "catch.hpp"
#include <thread-utils/async_queue.h>

TEST_CASE("2: Queue starts out empty", "[queue]")
{

    toolbox::AsyncQueue<int> queue;
    REQUIRE(queue.empty() == true);
    REQUIRE(queue.queueSize() == 0);

    int item;
    REQUIRE(queue.getItem(item, 1) == false);
}

TEST_CASE("3: post/get", "[queue]")
{

    toolbox::AsyncQueue<int> queue;
    queue.postItem(64);
    REQUIRE(queue.empty() == false);
    REQUIRE(queue.queueSize() == 1);

    int item;

    REQUIRE(queue.getItem(item, 0) == true);
    REQUIRE(item == 64);
}
