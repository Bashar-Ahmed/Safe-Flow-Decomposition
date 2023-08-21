#pragma once

#include <memory>

struct Node {
    int value, children;
    double flow_to_parent;
    std::shared_ptr<Node> parent_node;
    Node(int u);
    Node(int u, double flow, std::shared_ptr<Node> parent);
};

struct Trie {
    std::shared_ptr<Node> head;
    Trie(int u);
    std::shared_ptr<Node> insert(int u, double flow, std::shared_ptr<Node> parent);
    void insert(std::unique_ptr<Trie>& u, double flow, std::shared_ptr<Node> parent);
};
