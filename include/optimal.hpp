#pragma once

#include <memory>
#include <iostream>
#include <algorithm>
#include "../include/graph.hpp"

struct Forest_Node;
struct Optimal : public Graph
{
    bool heuristics;

    std::vector<double> f_in, f_out, f_max_in, f_max_out;
    std::vector<int> v_max_in, v_max_out, c_max_in, c_max_out;
    std::vector<std::shared_ptr<Forest_Node>> forest_in, forest_out;

    std::vector<std::pair<double, std::vector<int>>> optimal_repr, optimal_repr_l, optimal_repr_r, trivial;

    Optimal(const std::string &graph, bool heuristics);
    int binary_search_1(Forest_Node *node, double flow);
    int binary_search_2(Forest_Node *node_1, Forest_Node *node_2);
    void compute_non_trivial();
    bool left_extendible(int node, double flow);
    bool right_extendible(int node, double flow);
    void compute_trivial();
    void print_maximal_safe_paths();

    void construct_forest();
    void dfs_in(int n, int &label);
    void dfs_out(int n, int &label);
};

struct Forest_Node
{
    int value, label, depth, parent;
    double flow, loss;
    std::vector<int> children;
    std::shared_ptr<std::vector<Forest_Node *>> label_map;
    std::shared_ptr<std::vector<std::vector<int>>> depth_map;
    std::vector<Forest_Node *> *fast_label_map;
    std::vector<std::vector<int>> *fast_depth_map;
    Forest_Node(int u) : value(u), parent(u), flow(0.0) {}

    void init_root()
    {
        loss = 0;
        depth = 0;
        depth_map = std::make_shared<std::vector<std::vector<int>>>();
        label_map = std::make_shared<std::vector<Forest_Node *>>();
        fast_depth_map = depth_map.get();
        fast_label_map = label_map.get();
    }

    void init_node(int label)
    {
        if (int(fast_depth_map->size()) == depth)
            fast_depth_map->emplace_back(std::move(std::vector<int>()));
        fast_depth_map->operator[](depth).emplace_back(label);
        fast_label_map->emplace_back(this);
        this->label = label;
    }

    void update_node(Forest_Node *node, double flow)
    {
        loss = node->loss + flow;
        depth = node->depth + 1;
        depth_map = node->depth_map;
        label_map = node->label_map;
        fast_depth_map = node->fast_depth_map;
        fast_label_map = node->fast_label_map;
    }

    int binary_search(std::vector<int> &nodes)
    {
        int l = 0;
        int r = int(nodes.size()) - 1;
        while (l < r)
        {
            int m = (l + r + 1) >> 1;
            if (nodes[m] > label)
                r = m - 1;
            else
                l = m;
        }
        return nodes[l];
    }

    Forest_Node *level_ancestor(int depth)
    {
        auto &nodes = fast_depth_map->operator[](depth);
        int label = binary_search(nodes);
        return fast_label_map->operator[](label);
    }
};
