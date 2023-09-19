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

    static int length;
    static int total_nodes;
    static int total_edges;
    static void print_statistics();
};

std::string read_graph();
