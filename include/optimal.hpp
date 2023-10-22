#pragma once

#include <iostream>
#include <algorithm>
#include "../include/graph.hpp"
#include "../include/node.hpp"

struct Optimal : public Graph
{
    std::vector<double> f_in, f_out, f_max_in, f_max_out;
    std::vector<int> v_max_in, v_max_out, c_max_in, c_max_out;
    std::vector<std::shared_ptr<Optimal_Node>> forest_in, forest_out;

    std::vector<std::pair<double, std::vector<int>>> optimal_repr;

    Optimal(const std::string &graph);
    int binary_search_1(Optimal_Node *node, double flow);
    int binary_search_2(Optimal_Node *node_1, Optimal_Node *node_2);
    void compute_non_trivial();
    bool left_extendible(int node, double flow);
    bool right_extendible(int node, double flow);
    void compute_trivial();
    void print_maximal_safe_paths();
    void calculate_statistics();
    void construct_forest();
    void dfs_in(int n, int &label);
    void dfs_out(int n, int &label);
};
