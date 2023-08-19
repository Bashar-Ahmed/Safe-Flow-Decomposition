#include <gtest/gtest.h>

#include "../test/truth.hpp"
#include "../include/graph.hpp"

TEST(GRAPH, Graph) {

    Graph graph(graph_string_truth);
    EXPECT_EQ(graph.nodes,nodes_truth);
    EXPECT_EQ(graph.edges,edges_truth);
}
