#pragma once

#include <iostream>
#include <algorithm>
#include "../include/graph.hpp"
#include "../include/trie.hpp"

typedef std::tuple<std::shared_ptr<Concise_Node>, int, double> cut;
typedef std::pair<std::vector<int>, std::vector<cut>> path;

struct Concise : public Graph
{
    std::vector<double> f_in, f_out;
    std::vector<double> f_max_in, f_max_out;
    std::vector<int> v_max_in, v_max_out, topo_order;
    std::vector<std::unique_ptr<Path_Trie<Concise_Node>>> trie;
    std::vector<std::vector<int>> mark;
    std::vector<std::vector<path>> partial_result;

    std::vector<path> concise_repr;

    Concise(const std::string &graph);
    void compute_safe(int u);
    void print_maximal_safe_paths();
    void calculate_statistics();
    void topo_dfs(int v, std::vector<bool> &visited);
};