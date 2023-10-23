#include <fstream>
#include <sstream>
#include <gtest/gtest.h>

#include "../include/optimal.hpp"

TEST(OPTIMAL, optimal)
{
    std::ifstream input_file, truth_file;

    input_file.open("../test/test.graph", std::ifstream::in);
    std::cin.rdbuf(input_file.rdbuf());
    std::vector<std::pair<double, std::vector<int>>> truth, optimal_result;

    std::string graph_string = Graph::read();
    Optimal *graph = new Optimal(graph_string);
    graph->compute_non_trivial();
    graph->compute_trivial();

    optimal_result = std::move(graph->optimal_repr);

    for (auto &&path : graph->optimal_repr_l)
    {
        path.second.insert(path.second.begin(), path.second[0]);
        optimal_result.push_back(std::move(path));
    }

    for (auto &&path : graph->optimal_repr_r)
    {
        path.second.push_back(path.second[2]);
        optimal_result.push_back(std::move(path));
    }

    for (auto &&path : optimal_result)
    {
        std::vector<int> complete_path;
        int l = path.second[0];
        int i = path.second[1];
        int j = path.second[2];
        int r = path.second[3];

        complete_path.push_back(i);
        while (i != l)
        {
            i = graph->forest_in[i]->parent;
            complete_path.push_back(i);
        }
        std::reverse(complete_path.begin(), complete_path.end());
        complete_path.push_back(j);
        while (j != r)
        {
            j = graph->forest_out[j]->parent;
            complete_path.push_back(j);
        }
        path.second = std::move(complete_path);
    }

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

    std::sort(optimal_result.begin(), optimal_result.end());
    std::sort(truth.begin(), truth.end());

    EXPECT_EQ(optimal_result, truth);
}
