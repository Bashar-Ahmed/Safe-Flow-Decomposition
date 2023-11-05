#include <fstream>
#include <sstream>
#include <gtest/gtest.h>

#include "../include/concise.hpp"

TEST(CONCISE, concise)
{
    std::ifstream input_file, truth_file;

    input_file.open("../test/test.graph", std::ifstream::in);
    std::cin.rdbuf(input_file.rdbuf());
    std::vector<std::pair<double, std::vector<int>>> truth, concise_result;

    std::string graph_string = Graph::read();
    Concise *graph = new Concise(graph_string);
    for (int u : graph->topo_order)
        graph->compute_safe(u);

    for (auto &&path_instance : graph->concise_repr)
    {
        auto it = path_instance.first.begin();
        for (auto &&cut_instance : path_instance.second)
        {
            int u = std::get<0>(cut_instance)->value;
            int v = std::get<1>(cut_instance);
            double flow = std::get<2>(cut_instance);
            while (*it != u)
                it++;
            std::vector<int> path;
            auto p = it;
            while (*p != v)
                path.push_back(*p++);
            path.push_back(v);
            concise_result.emplace_back(std::make_pair(flow, std::move(path)));
        }
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

    std::sort(concise_result.begin(), concise_result.end());
    std::sort(truth.begin(), truth.end());

    EXPECT_EQ(concise_result, truth);
}
