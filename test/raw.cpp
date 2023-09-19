#include <fstream>
#include <sstream>
#include <gtest/gtest.h>

#include "../include/raw.hpp"

TEST(RAW, raw)
{

    freopen("../test/test.graph", "r", stdin);

    std::vector<std::pair<double, std::vector<int>>> truth, raw_result;

    std::string graph_string = read_graph();
    Raw *graph = new Raw(graph_string);
    for (int u : graph->topo_order)
        graph->compute_safe(u);
    raw_result = std::move(graph->raw_repr);

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

    std::sort(raw_result.begin(), raw_result.end());
    std::sort(truth.begin(), truth.end());

    EXPECT_EQ(raw_result, truth);
}
