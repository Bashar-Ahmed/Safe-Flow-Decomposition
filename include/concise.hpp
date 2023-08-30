#pragma once

#include <iostream>
#include <algorithm>
#include "../include/graph.hpp"
#include "../include/trie.hpp"

typedef std::tuple<std::shared_ptr<Node>, int, double> index;
typedef std::pair<std::list<int>, std::list<index>> path_index;

struct Concise : public Graph
{
    std::vector<double> f_in, f_out;
    std::vector<double> f_max_in, f_max_out;
    std::vector<int> v_max_in, v_max_out, topo_order;
    std::vector<std::unique_ptr<Trie>> trie;
    std::vector<std::queue<int>> mark;
    std::vector<std::list<path_index>> partial_result;

    std::list<path_index> concise_repr;

    Concise(const std::string &graph);
    void compute_safe(int u);
    void print_maximal_safe_paths();
    void calculate_statistics();
    void topo_dfs(int v, std::vector<bool> &visited);
};