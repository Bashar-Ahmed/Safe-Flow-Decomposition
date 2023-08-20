#pragma once

#include <sstream>
#include <string>
#include <vector>
#include <forward_list>

struct Graph {
    int nodes, edges, source, sink;
    std::string metadata;
    std::vector<std::forward_list<std::pair<int,double>>> adjacency_list;

    Graph(const std::string& graph);
};
