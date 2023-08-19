#include <gtest/gtest.h>

#include "../test/truth.hpp"
#include "../include/io.hpp"

TEST(IO, read_graph) {

    freopen("../test/test.graph", "r", stdin);

    while(true) {
        std::pair<std::string,bool> status = read_graph();
        std::string graph_string = status.first;
        if(graph_string=="\n") continue;

        EXPECT_EQ(graph_string, graph_string_truth);

        if(!status.second) break;
    }
}
