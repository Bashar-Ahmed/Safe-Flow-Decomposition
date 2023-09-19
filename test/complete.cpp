#include <fstream>
#include <sstream>
#include <gtest/gtest.h>

#include "../include/complete.hpp"

TEST(COMPLETE, complete)
{

    freopen("../test/test.graph", "r", stdin);

    std::vector<std::pair<double, std::vector<int>>> truth, complete_result;

    std::string graph_string = read_graph();
    Complete *graph = new Complete(graph_string);
    graph->decompose_path();
    graph->compute_safe();
    complete_result = std::move(graph->complete_repr);

    std::ifstream truth_file;
    truth_file.open("../test/test.truth", std::ifstream::in);
    std::string metadata, line, token;
    std::getline(truth_file, metadata);

    while (std::getline(truth_file, line))
    {
        std::pair<double, std::vector<int>> path;
        std::stringstream tokens(line);
        std::getline(tokens, token, ' ');
        path.first = stod(token);
        while (std::getline(tokens, token, ' '))
            path.second.push_back(stoi(token));
        truth.push_back(std::move(path));
    }

    EXPECT_EQ(complete_result, truth);
}
