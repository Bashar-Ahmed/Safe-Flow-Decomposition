#pragma once

#include <iostream>
#include <vector>
#include <forward_list>
#include <algorithm>
#include "../include/graph.hpp"
#include "../include/trie.hpp"

struct Raw_Graph : public Graph {
    std::vector<double> f_max_in, f_max_out, f_in, f_out;
    std::vector<int> v_max_in, v_max_out, topo_order;
    std::vector<Trie*> trie;
    std::vector<std::forward_list<std::pair<Node*,double>>> leaves;

    std::forward_list<std::pair<double,std::forward_list<int>>> raw_decomposition;

    Raw_Graph(const std::string& graph);
    void compute_safe(int u);
    void print_raw_decomposition();
    void topo_dfs(int v, std::vector<bool>& visited);
};
