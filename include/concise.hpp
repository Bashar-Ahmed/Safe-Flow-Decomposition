#pragma once

#include <iostream>
#include <algorithm>
#include "../include/graph.hpp"
#include "../include/trie.hpp"

typedef std::tuple<std::shared_ptr<Concise_Node>, int, double> route;
typedef std::tuple<int, int, double> cut;
template <typename T>
using path = std::pair<std::vector<int>, std::vector<T>>;

struct Concise : public Graph
{
    bool heuristics;

    std::vector<double> f_in;
    std::vector<double> f_max_in;
    std::vector<int> v_max_in, topo_order;
    std::vector<std::unique_ptr<Path_Trie<Concise_Node>>> trie;
    std::vector<std::vector<int>> mark;
    std::vector<std::vector<path<route>>> partial_result;

    std::vector<path<cut>> concise_repr;

    Concise(const std::string &graph, bool heuristics);
    void compute_safe(int u);
    void print_maximal_safe_paths();

    void topo_dfs(int v, std::vector<bool> &visited);
};