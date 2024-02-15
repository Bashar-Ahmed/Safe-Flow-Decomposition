#include <fstream>
#include <sstream>
#include <gtest/gtest.h>

#include "../include/old.hpp"

TEST(OLD_RAW, old_raw)
{

    std::ifstream input_file, truth_file;
    input_file.open("../test/test.graph", std::ifstream::in);
    std::cin.rdbuf(input_file.rdbuf());

    std::vector<std::pair<double, std::vector<int>>> truth, old_raw_result;

    std::string graph_string = Graph::read();
    Old<RAW> *graph = new Old<RAW>(graph_string, true);
    graph->decompose_path();
    graph->compute_safe();
    old_raw_result = std::move(graph->raw_repr);

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

    EXPECT_EQ(old_raw_result, truth);
}
