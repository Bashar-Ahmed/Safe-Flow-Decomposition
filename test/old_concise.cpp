#include <fstream>
#include <sstream>
#include <gtest/gtest.h>

#include "../include/old_concise.hpp"

TEST(OLD_CONCISE, old_concise)
{

    std::ifstream input_file, truth_file;
    input_file.open("../test/test.graph", std::ifstream::in);
    std::cin.rdbuf(input_file.rdbuf());

    std::vector<std::pair<double, std::vector<int>>> truth, old_concise_result;

    std::string graph_string = Graph::read();
    Old_Concise *graph = new Old_Concise(graph_string);
    graph->decompose_path();
    graph->compute_safe();
    old_concise_result = std::move(graph->old_concise_repr);

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

    EXPECT_EQ(old_concise_result, truth);
}
