#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "LinkedList.hpp"
#include "doctest.h"

TEST_CASE("Inserting nodes.") {
    mtrn3100::LinkedList<int> ll;
    REQUIRE(ll.size() == 0);

    ll.insert(0, 42);
    CHECK(ll.size() == 1);
    CHECK(ll[0] == 42);

    ll.insert(0, 63);
    CHECK(ll.size() == 2);
    CHECK(ll[0] == 63);
    CHECK(ll[1] == 42);

    ll.insert(1, 54);
    CHECK(ll.size() == 3);
    CHECK(ll[0] == 63);
    CHECK(ll[1] == 54);
    CHECK(ll[2] == 42);

    ll.insert(3, 89);
    CHECK(ll.size() == 4);
    CHECK(ll[0] == 63);
    CHECK(ll[1] == 54);
    CHECK(ll[2] == 42);
    CHECK(ll[3] == 89);
}

TEST_CASE("Access front and back.") {
    mtrn3100::LinkedList<int> ll;
    ll.insert(0, 42);
    ll.insert(0, 63);
    ll.insert(1, 54);
    ll.insert(3, 89);

    CHECK(ll.front() == 63);
    CHECK(ll.back() == 89);
}

TEST_CASE("Removing nodes.") {
    mtrn3100::LinkedList<int> ll;
    ll.insert(0, 42);
    ll.insert(0, 63);
    ll.insert(1, 54);
    ll.insert(3, 89);
    REQUIRE(ll.size() == 4);
    REQUIRE(ll[0] == 63);
    REQUIRE(ll[1] == 54);
    REQUIRE(ll[2] == 42);
    REQUIRE(ll[3] == 89);

    CHECK(ll.remove(3) == 89);
    CHECK(ll.size() == 3);
    CHECK(ll[0] == 63);
    CHECK(ll[1] == 54);
    CHECK(ll[2] == 42);

    CHECK(ll.remove(1) == 54);
    CHECK(ll.size() == 2);
    CHECK(ll[0] == 63);
    CHECK(ll[1] == 42);

    CHECK(ll.remove(0) == 63);
    CHECK(ll.size() == 1);
    CHECK(ll[0] == 42);

    CHECK(ll.remove(0) == 42);
    CHECK(ll.size() == 0);
}

TEST_CASE("Clearing nodes.") {
    mtrn3100::LinkedList<int> ll;
    ll.insert(0, 42);
    ll.insert(0, 63);
    ll.insert(1, 54);
    ll.insert(3, 89);
    REQUIRE(ll.size() == 4);

    ll.clear();
    CHECK(ll.size() == 0);
    CHECK(ll.empty() == true);
}

TEST_CASE("Pushing (front) nodes.") {
    mtrn3100::LinkedList<int> ll;
    ll.push_front(42);
    CHECK(ll.size() == 1);
    CHECK(ll[0] == 42);

    ll.push_front(53);
    CHECK(ll.size() == 2);
    CHECK(ll[0] == 53);
    CHECK(ll[1] == 42);
}

TEST_CASE("Popping (front) nodes.") {
    mtrn3100::LinkedList<int> ll;
    ll.push_front(42);
    ll.push_front(53);
    ll.push_front(64);
    REQUIRE(ll.size() == 3);
    REQUIRE(ll[0] == 64);
    REQUIRE(ll[1] == 53);
    REQUIRE(ll[2] == 42);

    CHECK(ll.pop_front() == 64);
    CHECK(ll.size() == 2);
    CHECK(ll.pop_front() == 53);
    CHECK(ll.size() == 1);
    CHECK(ll.pop_front() == 42);
    CHECK(ll.size() == 0);
}

TEST_CASE("Pushing (back) nodes.") {
    mtrn3100::LinkedList<int> ll;
    ll.push_back(42);
    CHECK(ll.size() == 1);
    CHECK(ll[0] == 42);

    ll.push_back(53);
    CHECK(ll.size() == 2);
    CHECK(ll[0] == 42);
    CHECK(ll[1] == 53);
}

TEST_CASE("Popping (back) nodes.") {
    mtrn3100::LinkedList<int> ll;
    ll.push_back(42);
    ll.push_back(53);
    ll.push_back(64);
    REQUIRE(ll.size() == 3);
    REQUIRE(ll[0] == 42);
    REQUIRE(ll[1] == 53);
    REQUIRE(ll[2] == 64);

    CHECK(ll.pop_back() == 64);
    CHECK(ll.size() == 2);
    CHECK(ll.pop_back() == 53);
    CHECK(ll.size() == 1);
    CHECK(ll.pop_back() == 42);
    CHECK(ll.size() == 0);
}

TEST_CASE("Copying linked list.") {
    mtrn3100::LinkedList<int> ll1;
    ll1.push_back(42);
    ll1.push_back(53);
    ll1.push_back(64);
    mtrn3100::LinkedList<int> ll2 = ll1;
    CHECK(ll1 == ll2);
}

TEST_CASE("Check if linked list contains element.") {
    mtrn3100::LinkedList<int> ll;
    ll.push_back(42);
    ll.push_back(53);
    CHECK(ll.contains(42) == true);
    CHECK(ll.contains(53) == true);
    CHECK(ll.contains(41) == false);
}

TEST_CASE("Accessing iterators of empty graph.") {
    mtrn3100::LinkedList<int> ll;
    CHECK(ll.begin() == ll.end());
}

TEST_CASE("Decrementing end iterator.") {
    mtrn3100::LinkedList<int> ll;
    ll.push_back(1);
    ll.push_back(2);
    ll.push_back(3);
    auto it = ll.end();
    --it;
    CHECK(it->value == 3);
    --it;
    CHECK(it->value == 2);
    --it;
    CHECK(it->value == 1);
}

TEST_CASE("Incrementing begin iterator.") {
    mtrn3100::LinkedList<int> ll;
    ll.push_back(1);
    ll.push_back(2);
    ll.push_back(3);
    auto it = ll.begin();
    CHECK(it->value == 1);
    ++it;
    CHECK(it->value == 2);
    ++it;
    CHECK(it->value == 3);
    ++it;
    CHECK(it == ll.end());
}

TEST_CASE("Initialising linked list.") {
    mtrn3100::LinkedList<int> ll(1, 2, 3, 2, 1, -1, 0);
    CHECK(ll[0] == 1);
    CHECK(ll[1] == 2);
    CHECK(ll[2] == 3);
    CHECK(ll[3] == 2);
    CHECK(ll[4] == 1);
    CHECK(ll[5] == -1);
    CHECK(ll[6] == 0);
}

TEST_CASE("Insert with ascending order.") {
    mtrn3100::LinkedList<int> ll;

    ll.insert(4);
    CHECK(ll[0] == 4);

    ll.insert(3);
    CHECK(ll[0] == 3);
    CHECK(ll[1] == 4);

    ll.insert(5);
    CHECK(ll[0] == 3);
    CHECK(ll[1] == 4);
    CHECK(ll[2] == 5);

    ll.insert(1);
    CHECK(ll[0] == 1);
    CHECK(ll[1] == 3);
    CHECK(ll[2] == 4);
    CHECK(ll[3] == 5);

    ll.insert(0);
    CHECK(ll[0] == 0);
    CHECK(ll[1] == 1);
    CHECK(ll[2] == 3);
    CHECK(ll[3] == 4);
    CHECK(ll[4] == 5);

    ll.insert(2);
    CHECK(ll[0] == 0);
    CHECK(ll[1] == 1);
    CHECK(ll[2] == 2);
    CHECK(ll[3] == 3);
    CHECK(ll[4] == 4);
    CHECK(ll[5] == 5);

    ll.insert(6);
    CHECK(ll[0] == 0);
    CHECK(ll[1] == 1);
    CHECK(ll[2] == 2);
    CHECK(ll[3] == 3);
    CHECK(ll[4] == 4);
    CHECK(ll[5] == 5);
    CHECK(ll[6] == 6);
}