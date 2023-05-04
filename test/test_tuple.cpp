#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "Tuple.hpp"
#include "doctest.h"

TEST_CASE("Single argument") {
    mtrn3100::Tuple<int> t;
    CHECK(mtrn3100::get<0>(t) == 0);
}

TEST_CASE("Multiple arguments") {
    mtrn3100::Tuple<int, int> t;
    CHECK(mtrn3100::get<0>(t) == 0);
    CHECK(mtrn3100::get<1>(t) == 0);
}

TEST_CASE("Multiple arguments with initialisation") {
    mtrn3100::Tuple<int, int> t{42, 43};
    CHECK(mtrn3100::get<0>(t) == 42);
    CHECK(mtrn3100::get<1>(t) == 43);
    mtrn3100::get<0>(t) = 142;
    mtrn3100::get<1>(t) = 143;
    CHECK(mtrn3100::get<0>(t) == 142);
    CHECK(mtrn3100::get<1>(t) == 143);
}

TEST_CASE("Equals") {
    mtrn3100::Tuple<int, float, char, float> lhs(1, 1.2, 'a', 2.2);
    mtrn3100::Tuple<int, float, char, float> rhs(1, 1.2, 'a', 2.2);
    CHECK(operator==(lhs, rhs));  // Cannot do CHECK(lhs == rhs);
}

TEST_CASE("Not equals") {
    mtrn3100::Tuple<int, float, char, float> lhs(1, 1.2, 'a', 2.2);
    mtrn3100::Tuple<int, float, char, float> rhs(1, 1.2, 'c', 2.2);
    CHECK(operator!=(lhs, rhs));  // Cannot do CHECK(lhs != rhs);
}