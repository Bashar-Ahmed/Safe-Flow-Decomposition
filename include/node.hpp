#pragma once

#include <memory>
#include <map>

struct Raw_Node
{
    int value, children;
    double flow;
    std::shared_ptr<Raw_Node> parent;
    Raw_Node(int u) : value(u), children(0), flow(0.0), parent(nullptr) {}
};

struct Concise_Node
{
    int value, children;
    double flow;
    std::shared_ptr<Concise_Node> parent;
    std::map<int, std::shared_ptr<Concise_Node>> child;
    Concise_Node(int u) : value(u), children(0), flow(0.0), parent(nullptr) {}
};

struct Optimal_Node
{
    int value, label, depth, parent;
    double flow, loss;
    std::vector<int> children;
    std::shared_ptr<std::vector<std::shared_ptr<Optimal_Node>>> label_map;
    std::shared_ptr<std::vector<std::vector<int>>> depth_map;
    Optimal_Node(int u) : value(u), parent(u), flow(0.0) {}

    std::shared_ptr<Optimal_Node> level_ancestor(int depth)
    {
        auto &nodes = this->depth_map->operator[](depth);
        int label = *(--std::upper_bound(nodes.begin(), nodes.end(), this->label));
        return label_map->operator[](label);
    }
};
