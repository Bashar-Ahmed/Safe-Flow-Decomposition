#pragma once

#include <sstream>
#include <string>
#include <vector>
#include <list>

struct Graph {
    int nodes, edges;
    std::string metadata;
    std::vector<double> f_in, f_out;
    std::vector<std::list<std::pair<int,double>>> adjacency_list;

    Graph(const std::string& graph);
};
