#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>

struct Graph
{
    int nodes, edges;
    std::string metadata;
    std::vector<std::vector<std::pair<int, double>>> adjacency_list;

    Graph(const std::string &graph);
    virtual ~Graph() {}
    virtual void print_maximal_safe_paths() {}
    virtual void calculate_statistics() {}

    static inline int length = 0;
    static inline int total_nodes = 0;
    static inline int total_edges = 0;
    static void print_statistics();
};
