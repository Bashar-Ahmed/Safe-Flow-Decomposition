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
    std::shared_ptr<std::vector<Optimal_Node *>> label_map;
    std::shared_ptr<std::vector<std::vector<int>>> depth_map;
    std::vector<Optimal_Node *> *fast_label_map;
    std::vector<std::vector<int>> *fast_depth_map;
    Optimal_Node(int u) : value(u), parent(u), flow(0.0) {}

    inline void init_root()
    {
        loss = 0;
        depth = 0;
        depth_map = std::make_shared<std::vector<std::vector<int>>>();
        label_map = std::make_shared<std::vector<Optimal_Node *>>();
        fast_depth_map = depth_map.get();
        fast_label_map = label_map.get();
    }

    inline void init_node(int label)
    {
        if (int(fast_depth_map->size()) == depth)
            fast_depth_map->emplace_back(std::move(std::vector<int>()));
        fast_depth_map->operator[](depth).emplace_back(label);
        fast_label_map->emplace_back(this);
        this->label = label;
    }

    inline void update_node(std::shared_ptr<Optimal_Node> &node, double flow)
    {
        loss = node->loss + flow;
        depth = node->depth + 1;
        depth_map = node->depth_map;
        label_map = node->label_map;
        fast_depth_map = node->fast_depth_map;
        fast_label_map = node->fast_label_map;
    }

    inline int binary_search(std::vector<int> &nodes)
    {
        int l = 0;
        int r = int(nodes.size()) - 1;
        int ans = l;
        while (l <= r)
        {
            int m = (l + r) >> 1;
            if (nodes[m] > label)
                r = m - 1;
            else
            {
                ans = m;
                l = m + 1;
            }
        }
        return nodes[ans];
    }

    inline Optimal_Node *level_ancestor(int depth)
    {
        auto &nodes = fast_depth_map->operator[](depth);
        int label = binary_search(nodes);
        return fast_label_map->operator[](label);
    }
};
