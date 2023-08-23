#pragma once

#include <iostream>
#include <algorithm>
#include "../include/graph.hpp"
#include "../include/trie.hpp"

struct Raw : public Graph {
    std::vector<double> f_max_in, f_max_out;
    std::vector<int> v_max_in, v_max_out, topo_order;
    std::vector<std::unique_ptr<Trie>> trie;
    std::vector<std::list<std::pair<std::shared_ptr<Node>,double>>> leaves;

    std::list<std::pair<double,std::list<int>>> raw_repr;

    Raw(const std::string& graph);
    void compute_safe(int u);
    void print_maximal_safe_paths();
    void topo_dfs(int v, std::vector<bool>& visited);
};
