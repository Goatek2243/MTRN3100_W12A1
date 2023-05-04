#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "Graph.hpp"
#include "doctest.h"

TEST_CASE("Empty graph.") {
    auto g = mtrn3100::Graph<int, int>();
    CHECK(g.empty());
}

TEST_CASE("Check if node exists.") {
    auto g = mtrn3100::Graph<int, bool>();
    g.insert_node(3, 4, 6, 8, -4, -1, 0, 23, 53, -59);
    CHECK(g.is_node(3));
    CHECK(g.is_node(4));
    CHECK(g.is_node(6));
    CHECK(g.is_node(8));
    CHECK(g.is_node(-4));
    CHECK(g.is_node(-1));
    CHECK(g.is_node(0));
    CHECK(g.is_node(23));
    CHECK(g.is_node(53));
    CHECK(g.is_node(-59));
}

TEST_CASE("Check if node does not exist.") {
    auto g = mtrn3100::Graph<int, bool>(3, 4, 6, 8, -4, -1, 0, 23, 53, -59);
    CHECK_FALSE(g.is_node(1));
    CHECK_FALSE(g.is_node(2));
    CHECK_FALSE(g.is_node(5));
    CHECK_FALSE(g.is_node(7));
    CHECK_FALSE(g.is_node(-2));
    CHECK_FALSE(g.is_node(59));
    CHECK_FALSE(g.is_node(-53));
}

TEST_CASE("Inserting unique nodes.") {
    auto g = mtrn3100::Graph<int, bool>();
    REQUIRE(g.empty());
    CHECK(g.insert_node(1));
    CHECK(g.is_node(1));
    CHECK(g.insert_node(2));
    CHECK(g.is_node(2));
    CHECK(g.insert_node(123));
    CHECK(g.is_node(123));
}

TEST_CASE("Inserting duplicate nodes.") {
    auto g = mtrn3100::Graph<int, bool>(1, 2, 123);
    REQUIRE(g.is_node(1));
    REQUIRE(g.is_node(2));
    REQUIRE(g.is_node(123));
    CHECK_FALSE(g.insert_node(1));
    CHECK_FALSE(g.insert_node(2));
    CHECK_FALSE(g.insert_node(123));
}

TEST_CASE("Clearing graph.") {
    auto g = mtrn3100::Graph<int, int>(1, 2, 3);
    REQUIRE_FALSE(g.empty());
    g.clear();
    CHECK(g.empty());
}

TEST_CASE("Check graph with empty size.") {
    auto g = mtrn3100::Graph<int, int>();
    CHECK_EQ(0, g.size());
}

TEST_CASE("Check graph with non-empty size.") {
    auto g = mtrn3100::Graph<int, int>(1, 2);
    CHECK_EQ(2, g.size());
    g.insert_edge(1, 2, 1);  // Should have no effect on size.
    CHECK_EQ(2, g.size());
}

TEST_CASE("Erase existing node.") {
    auto g = mtrn3100::Graph<int, int>(1, 2);
    CHECK(g.erase_node(1));
    CHECK_EQ(1, g.size());
    CHECK(g.erase_node(2));
    CHECK_EQ(0, g.size());
}

TEST_CASE("Erase nonexisting node.") {
    auto g = mtrn3100::Graph<int, int>();
    CHECK_FALSE(g.erase_node(1));
    CHECK_FALSE(g.erase_node(0));
}

TEST_CASE("Inserting unique edges.") {
    auto g = mtrn3100::Graph<int, int>(1, 2, 3, 4, 5);
    CHECK(g.insert_edge(1, 2, 42));
    CHECK_NE(g.end(), g.find(1, 2, 42));
    CHECK(g.insert_edge(2, 3, 43));
    CHECK_NE(g.end(), g.find(2, 3, 43));
    CHECK(g.insert_edge(2, 3, 44));
    CHECK_NE(g.end(), g.find(2, 3, 44));
    CHECK(g.insert_edge(1, 1, 44));
    CHECK_NE(g.end(), g.find(1, 1, 44));
}

TEST_CASE("Inserting duplicate edges.") {
    auto g = mtrn3100::Graph<int, int>(1, 2, 3, 4, 5);
    CHECK(g.insert_edge(1, 2, 42));
    CHECK_NE(g.end(), g.find(1, 2, 42));
    CHECK_FALSE(g.insert_edge(1, 2, 42));
}

// TEST_CASE("Insert edge with missing nodes.") {
//     auto g = mtrn3100::Graph<int, int>(1, 2, 3, 4, 5);
//     CHECK_THROWS(g.insert_edge(0, 5, 43));
//     CHECK_THROWS(g.insert_edge(1, 0, 42));
// }

TEST_CASE("Erase existing node with edges.") {
    auto g = mtrn3100::Graph<int, int>({1, 2});
    REQUIRE(g.insert_edge(1, 2, 0));
    REQUIRE(g.insert_edge(2, 1, 0));
    CHECK(g.erase_node(1));
    CHECK_EQ(g.end(), g.find(1, 2, 0));
    CHECK_EQ(g.end(), g.find(2, 1, 0));
}

TEST_CASE("Check begin and end iterator of empty graph.") {
    auto g = mtrn3100::Graph<int, bool>();
    CHECK_EQ(g.end(), g.begin());
}

TEST_CASE("Check begin and end iterator of graph with only nodes.") {
    auto g = mtrn3100::Graph<int, bool>(1, 2, 3);
    CHECK_EQ(g.end(), g.begin());  // Note that dereferencing this iterator is invalid.
}

TEST_CASE("Check begin iterator with nodes and edges.") {
    auto g = mtrn3100::Graph<int, int>(1, 2, 3);
    REQUIRE(g.insert_edge(1, 2, 1));
    CHECK_NE(g.end(), g.begin());
    CHECK_EQ(1, (mtrn3100::get<0>(*g.begin())));
    CHECK_EQ(2, (mtrn3100::get<1>(*g.begin())));
    CHECK_EQ(1, (mtrn3100::get<2>(*g.begin())));
}

TEST_CASE("Check end iterator with nodes and edges.") {
    auto g = mtrn3100::Graph<int, int>(1, 2, 3);
    REQUIRE(g.insert_edge(1, 2, 1));
    REQUIRE(g.insert_edge(1, 2, 3));
    REQUIRE_NE(g.end(), g.begin());
    auto it = g.end();
    --it;
    CHECK_EQ(1, (mtrn3100::get<0>(*it)));
    CHECK_EQ(2, (mtrn3100::get<1>(*it)));
    CHECK_EQ(3, (mtrn3100::get<2>(*it)));
    --it;
    CHECK_EQ(1, (mtrn3100::get<0>(*it)));
    CHECK_EQ(2, (mtrn3100::get<1>(*it)));
    CHECK_EQ(1, (mtrn3100::get<2>(*it)));
}

TEST_CASE("Finding existing edges.") {
    auto g = mtrn3100::Graph<int, int>(1, 2, 3, 4, 5);
    REQUIRE(g.insert_edge(1, 5, 1230));
    REQUIRE(g.insert_edge(1, 1, -100));
    REQUIRE(g.insert_edge(1, 5, 130));
    auto it1 = g.find(1, 5, 1230);
    CHECK_NE(g.end(), it1);
    CHECK_EQ(1, mtrn3100::get<0>(*it1));
    CHECK_EQ(5, mtrn3100::get<1>(*it1));
    CHECK_EQ(1230, mtrn3100::get<2>(*it1));
    auto it2 = g.find(1, 1, -100);
    CHECK_NE(g.end(), it2);
    CHECK_EQ(1, mtrn3100::get<0>(*it2));
    CHECK_EQ(1, mtrn3100::get<1>(*it2));
    CHECK_EQ(-100, mtrn3100::get<2>(*it2));
    auto it3 = g.find(1, 5, 130);
    CHECK_NE(g.end(), it3);
    CHECK_EQ(1, mtrn3100::get<0>(*it3));
    CHECK_EQ(5, mtrn3100::get<1>(*it3));
    CHECK_EQ(130, mtrn3100::get<2>(*it3));
}

TEST_CASE("Finding nonexisting edges.") {
    auto g = mtrn3100::Graph<int, int>(1, 2, 3, 4, 5);
    REQUIRE(g.insert_edge(1, 5, 1230));
    REQUIRE(g.insert_edge(1, 1, -100));
    REQUIRE(g.insert_edge(1, 5, 130));
    auto it1 = g.find(1, 5, 1231);
    CHECK_EQ(g.end(), it1);
    auto it2 = g.find(1, 2, -100);
    CHECK_EQ(g.end(), it2);
    auto it3 = g.find(5, 1, 130);
    CHECK_EQ(g.end(), it3);
}

TEST_CASE("Dereferenced iterator.") {
    auto g = mtrn3100::Graph<int, int>(3, 2, 1);
    REQUIRE(g.insert_edge(1, 2, 4));
    auto it = g.find(1, 2, 4);
    CHECK_EQ(1, mtrn3100::get<0>(*it));
    CHECK_EQ(2, mtrn3100::get<1>(*it));
    CHECK_EQ(4, mtrn3100::get<2>(*it));
}

TEST_CASE("Check if nodes are connected.") {
    auto g = mtrn3100::Graph<int, int>(42, 43, 44, 45);
    REQUIRE(g.insert_edge(43, 45, 10));
    CHECK(g.is_connected(43, 45));
    REQUIRE(g.insert_edge(43, 45, 0));
    CHECK(g.is_connected(43, 45));
    REQUIRE(g.insert_edge(45, 43, 0));
    CHECK(g.is_connected(45, 43));
    REQUIRE(g.insert_edge(42, 42, 0));
    CHECK(g.is_connected(42, 42));
}

TEST_CASE("Check if nodes are not connected.") {
    auto g = mtrn3100::Graph<int, int>(42, 43, 44, 45);
    REQUIRE(g.insert_edge(43, 45, 10));
    CHECK_FALSE(g.is_connected(45, 43));
    CHECK_FALSE(g.is_connected(45, 45));
    CHECK_FALSE(g.is_connected(43, 43));
    CHECK_FALSE(g.is_connected(42, 42));
    CHECK_FALSE(g.is_connected(44, 42));
}

// TEST_CASE("Check connections of nonexisting nodes.") {
//     auto g = mtrn3100::Graph<int, int>(42, 43, 44, 45);
//     CHECK_THROWS(static_cast<void>(g.is_connected(42, -42)));
//     CHECK_THROWS(static_cast<void>(g.is_connected(-43, 43)));
// }

TEST_CASE("Erase existing edges.") {
    auto g = mtrn3100::Graph<int, int>(42, 43);
    REQUIRE(g.insert_edge(42, 43, 1));
    REQUIRE(g.insert_edge(43, 42, 1));
    REQUIRE(g.insert_edge(42, 43, 2));
    CHECK(g.erase_edge(42, 43, 1));
    CHECK_FALSE(g.erase_edge(42, 43, 1));
    CHECK_EQ(g.end(), g.find(42, 43, 1));
    CHECK_NE(g.end(), g.find(43, 42, 1));
    CHECK_NE(g.end(), g.find(42, 43, 2));
    CHECK(g.erase_edge(43, 42, 1));
    CHECK_FALSE(g.erase_edge(43, 42, 1));
    CHECK_EQ(g.end(), g.find(42, 43, 1));
    CHECK_EQ(g.end(), g.find(43, 42, 1));
    CHECK_NE(g.end(), g.find(42, 43, 2));
    CHECK(g.erase_edge(42, 43, 2));
    CHECK_FALSE(g.erase_edge(42, 43, 2));
    CHECK_EQ(g.end(), g.find(42, 43, 1));
    CHECK_EQ(g.end(), g.find(43, 42, 1));
    CHECK_EQ(g.end(), g.find(42, 43, 2));
}

TEST_CASE("Erase nonexisting edges.") {
    auto g = mtrn3100::Graph<int, int>(42, 43);
    CHECK_FALSE(g.erase_edge(43, 42, 1));
    CHECK_FALSE(g.erase_edge(42, 43, 2));
    REQUIRE(g.insert_edge(42, 42, 2));
    CHECK_FALSE(g.erase_edge(42, 42, 1));
}

// TEST_CASE("Erase nonexisting edge with nonexisting node.") {
//     auto g = mtrn3100::Graph<int, int>(42, 43);
//     CHECK_THROWS(g.erase_edge(44, 43, 1));
//     CHECK_THROWS(g.erase_edge(42, 44, 1));
// }

TEST_CASE("Erase existing edge with single iterator.") {
    auto g = mtrn3100::Graph<int, int>(1, 2, 3);
    REQUIRE(g.insert_edge(1, 1, 10));
    REQUIRE(g.insert_edge(1, 2, 10));
    REQUIRE(g.insert_edge(1, 3, 10));
    auto it1 = g.find(1, 1, 10);
    auto it2 = g.find(1, 2, 10);
    auto it3 = g.find(1, 3, 10);
    CHECK_EQ(it2, g.erase_edge(it1));
    CHECK_EQ(g.end(), g.find(1, 1, 10));
    CHECK_EQ(it3, g.erase_edge(it2));
    CHECK_EQ(g.end(), g.find(1, 2, 10));
    CHECK_EQ(g.end(), g.erase_edge(it3));
    CHECK_EQ(g.end(), g.find(1, 3, 10));
}

TEST_CASE("Erase nonexisting edge with single iterator.") {
    auto g = mtrn3100::Graph<int, int>(1, 2, 3);
    auto it1 = g.find(1, 1, 10);
    CHECK_EQ(g.end(), g.erase_edge(it1));
    auto it2 = g.find(1, 2, 10);
    CHECK_EQ(g.end(), g.erase_edge(it2));
    auto it3 = g.find(1, 3, 10);
    CHECK_EQ(g.end(), g.erase_edge(it3));
}

TEST_CASE("Check nodes of graph are sorted.") {
    auto g = mtrn3100::Graph<int, int>(0, 1, 3, -1, 2);
    auto v = mtrn3100::LinkedList<int>(-1, 0, 1, 2, 3);
    CHECK_EQ(g.nodes(), v);
}

TEST_CASE("Erase existing edges in valid range.") {
    auto g = mtrn3100::Graph<int, int>(1, 2, 3);
    REQUIRE(g.insert_edge(3, 2, 10));
    REQUIRE(g.insert_edge(3, 2, 4));
    REQUIRE(g.insert_edge(1, 3, 3));
    REQUIRE(g.insert_edge(1, 1, 10));
    REQUIRE(g.insert_edge(3, 3, 4));
    REQUIRE(g.insert_edge(2, 3, 3));
    REQUIRE(g.insert_edge(1, 2, 10));
    REQUIRE(g.insert_edge(1, 3, 10));
    auto first = g.find(1, 3, 3);
    auto last = g.find(3, 2, 10);
    CHECK_EQ(last, g.erase_edge(first, last));
    CHECK_NE(g.end(), g.find(1, 1, 10));
    CHECK_NE(g.end(), g.find(1, 2, 10));
    CHECK_EQ(g.end(), g.find(1, 3, 3));
    CHECK_EQ(g.end(), g.find(1, 3, 10));
    CHECK_EQ(g.end(), g.find(2, 3, 3));
    CHECK_EQ(g.end(), g.find(3, 2, 4));
    CHECK_NE(g.end(), g.find(3, 2, 10));
    CHECK_NE(g.end(), g.find(3, 3, 4));
}

TEST_CASE("Erase edges with nonexistent edges in range.") {
    auto g = mtrn3100::Graph<int, int>(1, 2, 3);
    REQUIRE(g.insert_edge(3, 2, 10));
    REQUIRE(g.insert_edge(3, 2, 4));
    REQUIRE(g.insert_edge(1, 3, 3));
    REQUIRE(g.insert_edge(1, 1, 10));
    REQUIRE(g.insert_edge(3, 3, 4));
    REQUIRE(g.insert_edge(2, 3, 3));
    REQUIRE(g.insert_edge(1, 2, 10));
    REQUIRE(g.insert_edge(1, 3, 10));
    auto first = g.find(1, 3, 0);
    auto last = g.find(3, 3, 4);
    CHECK_EQ(g.end(), g.erase_edge(first, last));
}

TEST_CASE("Erase edges with nonexistent end range.") {
    auto g = mtrn3100::Graph<int, int>(1, 2, 3);
    REQUIRE(g.insert_edge(3, 2, 10));
    REQUIRE(g.insert_edge(3, 2, 4));
    REQUIRE(g.insert_edge(1, 3, 3));
    REQUIRE(g.insert_edge(1, 1, 10));
    REQUIRE(g.insert_edge(3, 3, 4));
    REQUIRE(g.insert_edge(2, 3, 3));
    REQUIRE(g.insert_edge(1, 2, 10));
    REQUIRE(g.insert_edge(1, 3, 10));
    auto first = g.find(1, 3, 3);
    auto last = g.find(4, 5, 10);
    CHECK_EQ(g.end(), g.erase_edge(first, last));
    CHECK_NE(g.end(), g.find(1, 1, 10));
    CHECK_NE(g.end(), g.find(1, 2, 10));
    CHECK_EQ(g.end(), g.find(1, 3, 3));
    CHECK_EQ(g.end(), g.find(1, 3, 10));
    CHECK_EQ(g.end(), g.find(2, 3, 3));
    CHECK_EQ(g.end(), g.find(3, 2, 4));
    CHECK_EQ(g.end(), g.find(3, 2, 10));
    CHECK_EQ(g.end(), g.find(3, 3, 4));
}

TEST_CASE("Iterators are ordered.") {
    auto g = mtrn3100::Graph<int, int>(1, 2, 3);
    REQUIRE(g.insert_edge(3, 2, 10));
    REQUIRE(g.insert_edge(3, 2, 4));
    REQUIRE(g.insert_edge(1, 3, 3));
    REQUIRE(g.insert_edge(1, 1, 10));
    REQUIRE(g.insert_edge(3, 3, 4));
    REQUIRE(g.insert_edge(2, 3, 3));
    REQUIRE(g.insert_edge(1, 2, 10));
    REQUIRE(g.insert_edge(1, 3, 10));
    auto iter = g.begin();
    CHECK_EQ(iter++, g.find(1, 1, 10));
    CHECK_EQ(iter++, g.find(1, 2, 10));
    CHECK_EQ(iter++, g.find(1, 3, 3));
    CHECK_EQ(iter++, g.find(1, 3, 10));
    CHECK_EQ(iter++, g.find(2, 3, 3));
    CHECK_EQ(iter++, g.find(3, 2, 4));
    CHECK_EQ(iter++, g.find(3, 2, 10));
    CHECK_EQ(iter++, g.find(3, 3, 4));
    CHECK_EQ(iter, g.end());
}

TEST_CASE("Check weights with graph with no edges.") {
    auto g = mtrn3100::Graph<int, int>(1, 2, 3);
    auto v = g.weights(1, 2);
    CHECK_EQ(0, v.size());
}

TEST_CASE("Check weights with nonexisting edges.") {
    auto g = mtrn3100::Graph<int, int>(1, 2, 3);
    REQUIRE(g.insert_edge(3, 2, 10));
    REQUIRE(g.insert_edge(3, 2, 4));
    REQUIRE(g.insert_edge(3, 2, 11));
    REQUIRE(g.insert_edge(1, 3, 3));
    auto v = g.weights(2, 3);
    CHECK_EQ(0, v.size());
}

TEST_CASE("Check weights with existing edges.") {
    auto g = mtrn3100::Graph<int, int>(1, 2, 3);
    REQUIRE(g.insert_edge(3, 2, 10));
    REQUIRE(g.insert_edge(3, 2, 4));
    REQUIRE(g.insert_edge(3, 2, 11));
    REQUIRE(g.insert_edge(1, 3, 3));
    auto v = g.weights(3, 2);
    CHECK_EQ(4, v[0]);
    CHECK_EQ(10, v[1]);
    CHECK_EQ(11, v[2]);
    CHECK_EQ(3, v.size());
}

// TEST_CASE("Check weights with nonexisting nodes.") {
//     auto g = mtrn3100::Graph<int, int>(1, 2, 3);
//     CHECK_THROWS(g.weights(3, 4));
//     CHECK_THROWS(g.weights(0, 1));
// }

TEST_CASE("Check nodes connected to src in graph with no edges.") {
    auto g = mtrn3100::Graph<int, int>(1, 2, 3);
    auto v = g.nodes(1);
    CHECK_EQ(0, v.size());
}

TEST_CASE("Check nodes connected to src in graph with existing edges.") {
    auto g = mtrn3100::Graph<int, int>({1, 2, 3, 10});
    REQUIRE(g.insert_edge(3, 2, 10));
    REQUIRE(g.insert_edge(3, 10, 4));
    REQUIRE(g.insert_edge(3, 3, 11));
    REQUIRE(g.insert_edge(1, 3, 3));
    auto v = g.nodes(3);
    CHECK_EQ(2, v[0]);
    CHECK_EQ(3, v[1]);
    CHECK_EQ(10, v[2]);
    CHECK_EQ(3, v.size());
}

TEST_CASE("Check nodes connected to src in graph with nonexisting edges.") {
    auto g = mtrn3100::Graph<int, int>({1, 2, 3, 10});
    REQUIRE(g.insert_edge(3, 2, 10));
    REQUIRE(g.insert_edge(3, 10, 4));
    REQUIRE(g.insert_edge(3, 3, 11));
    REQUIRE(g.insert_edge(1, 3, 3));
    auto v = g.nodes(2);
    CHECK_EQ(0, v.size());
}

// TEST_CASE("Check nodes connected to nonexisting node.") {
//     auto g = mtrn3100::Graph<int, int>(1, 2, 3, 10);
//     CHECK_THROWS(g.nodes(0));
// }

TEST_CASE("Comparing empty graphs.") {
    auto g1 = mtrn3100::Graph<int, int>();
    auto g2 = mtrn3100::Graph<int, int>();
    CHECK(g1 == g2);
}

TEST_CASE("Comparing equal graphs with only nodes.") {
    auto g1 = mtrn3100::Graph<int, int>(1, 2, 3);
    auto g2 = mtrn3100::Graph<int, int>(3, 2, 1);
    CHECK(g1 == g2);
}

TEST_CASE("Comparing equal graphs with edges.") {
    auto g1 = mtrn3100::Graph<int, int>(1, 2, 3);
    auto g2 = mtrn3100::Graph<int, int>(3, 2, 1);
    g1.insert_edge(1, 2, 1);
    g2.insert_edge(1, 2, 1);
    g1.insert_edge(1, 2, 3);
    g2.insert_edge(1, 2, 3);
    CHECK(g1 == g2);
}

TEST_CASE("Comparing unequal graphs with both graphs with edges.") {
    auto g1 = mtrn3100::Graph<int, int>(1, 2, 3);
    auto g2 = mtrn3100::Graph<int, int>(3, 2, 1);
    g1.insert_edge(1, 2, 1);
    g2.insert_edge(2, 1, 1);
    CHECK_FALSE(g1 == g2);
}

TEST_CASE("Comparing unequal graphs with one graph with edges.") {
    auto g1 = mtrn3100::Graph<int, int>(1, 2, 3);
    auto g2 = mtrn3100::Graph<int, int>(3, 2, 1);
    g2.insert_edge(1, 2, 1);
    g2.insert_edge(2, 1, 1);
    CHECK_FALSE(g1 == g2);
}

/*
TEST_CASE("Copy graph with only nodes.") {
    auto g1 = mtrn3100::Graph<int, int>();
    g1.insert_node(3, 4, 6, 8, -4, -1, 0);
    auto g2 = mtrn3100::Graph<int, int>(g1);
    CHECK(g1.is_node(3));
    CHECK(g1.is_node(4));
    CHECK(g1.is_node(6));
    CHECK(g1.is_node(8));
    CHECK(g1.is_node(-4));
    CHECK(g1.is_node(-1));
    CHECK(g1.is_node(0));
    CHECK(g2.is_node(3));
    CHECK(g2.is_node(4));
    CHECK(g2.is_node(6));
    CHECK(g2.is_node(8));
    CHECK(g2.is_node(-4));
    CHECK(g2.is_node(-1));
    CHECK(g2.is_node(0));
}

TEST_CASE("Copy graph with nodes and edges.") {
    auto g1 = mtrn3100::Graph<int, int>({3, 4, 6});
    g1.insert_edge(3, 4, 1);
    REQUIRE(g1.is_connected(3, 4));
    auto g2 = mtrn3100::Graph(g1);
    CHECK(g1.is_node(3));
    CHECK(g1.is_node(4));
    CHECK(g1.is_node(6));
    CHECK(g1.is_connected(3, 4));
    CHECK(g2.is_node(3));
    CHECK(g2.is_node(4));
    CHECK(g2.is_node(6));
    CHECK(g2.is_connected(3, 4));
}

TEST_CASE("Copy assign empty graph.") {
    auto g1 = mtrn3100::Graph<int, int>();
    auto g2 = mtrn3100::Graph<int, int>({0, -1, -2});
    g2 = g1;
    CHECK(g1.empty());
    CHECK(g2.empty());
}

TEST_CASE("Copy assign graph with only nodes.") {
    auto g1 = mtrn3100::Graph<int, int>({3, 4, 6});
    auto g2 = mtrn3100::Graph<int, int>({0, -1, -2});
    g2 = g1;
    CHECK(g1.is_node(3));
    CHECK(g1.is_node(4));
    CHECK(g1.is_node(6));
    CHECK(g2.is_node(3));
    CHECK(g2.is_node(4));
    CHECK(g2.is_node(6));
}

TEST_CASE("Copy assign graph with nodes and edges.") {
    auto g1 = mtrn3100::Graph<int, int>({3, 4, 6});
    g1.insert_edge(3, 4, 1);
    g1.insert_edge(3, 6, 1);
    REQUIRE(g1.is_connected(3, 4));
    REQUIRE(g1.is_connected(3, 6));
    auto g2 = mtrn3100::Graph<int, int>({0, -1, -2});
    g2 = g1;
    CHECK(g1.is_node(3));
    CHECK(g1.is_node(4));
    CHECK(g1.is_node(6));
    CHECK(g1.is_connected(3, 4));
    CHECK(g1.is_connected(3, 6));
    CHECK(g2.is_node(3));
    CHECK(g2.is_node(4));
    CHECK(g2.is_node(6));
    CHECK(g2.is_connected(3, 4));
    CHECK(g2.is_connected(3, 6));
}

// TEST_CASE(replace_node_with_nonexisting_node) {
//     auto g = mtrn3100::Graph<int, int>({1, 2});
//     REQUIRE(g.insert_edge(1, 2, 0));
//     CHECK(g.replace_node(2, 3));
//     CHECK_EQ(g.end(), g.find(1, 2, 0));
//     CHECK_NE(g.end(), g.find(1, 3, 0));
// }

// TEST_CASE(replace_node_with_existing_node) {
//     auto g = mtrn3100::Graph<int, int>(1, 2, 3);
//     REQUIRE(g.insert_edge(1, 2, 0));
//     CHECK_FALSE(g.replace_node(2, 3));
//     CHECK_EQ(g.end(), g.find(1, 2, 0));
//     CHECK_NE(g.end(), g.find(1, 3, 0));
// }

// TEST_CASE(replace_nonexisting_node) {
//     auto g = mtrn3100::Graph<int, int>({1, 2});
//     REQUIRE(g.insert_edge(1, 2, 0));
//     CHECK_THROWS_AS(g.replace_node(0, 3), std::runtime_error);
// }

// TEST_CASE(merge_replacing_existing_node) {
//     auto g = mtrn3100::Graph<int, int>({42, 43, 44, 45});
//     REQUIRE(g.insert_edge(42, 43, 1));
//     REQUIRE(g.insert_edge(42, 44, 2));
//     REQUIRE(g.insert_edge(42, 45, 3));
//     REQUIRE(g.insert_edge(43, 43, 1));
//     REQUIRE(g.insert_edge(44, 42, 1));
//     g.merge_replace_node(42, 43);
//     CHECK_NE(g.end(), g.find(43, 43, 1));
//     CHECK_EQ(g.end(), g.find(42, 43, 1));
//     CHECK_NE(g.end(), g.find(43, 44, 2));
//     CHECK_EQ(g.end(), g.find(42, 44, 2));
//     CHECK_NE(g.end(), g.find(43, 45, 3));
//     CHECK_EQ(g.end(), g.find(42, 45, 3));
//     CHECK_NE(g.end(), g.find(44, 43, 1));
//     CHECK_EQ(g.end(), g.find(44, 42, 1));
// }

// TEST_CASE(merge_replacing_nonexisting_node) {
//     auto g = mtrn3100::Graph<int, int>({42, 43});
//     CHECK_THROWS_AS(g.merge_replace_node(42, 44), std::runtime_error);
//     CHECK_THROWS_AS(g.merge_replace_node(44, 42), std::runtime_error);
// }

*/