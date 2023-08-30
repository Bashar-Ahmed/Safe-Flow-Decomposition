#pragma once

#include <memory>
#include <unordered_map>
#include <queue>
#include <list>

struct Node
{
    int value, children;
    double flow_to_parent;
    std::shared_ptr<Node> parent_node;
    std::unordered_map<int, std::weak_ptr<Node>> child;
    Node(int u);
    Node(int u, double flow, std::shared_ptr<Node> parent);
};

struct Trie
{
    std::shared_ptr<Node> head;
    Trie(int u);
    std::shared_ptr<Node> insert(int u, double flow, std::shared_ptr<Node> parent);
    void insert(std::unique_ptr<Trie> &u, double flow, std::shared_ptr<Node> parent);
};

struct AC_Trie : public std::enable_shared_from_this<AC_Trie>
{
    bool is_fail;
    int value;
    double flow;
    std::weak_ptr<AC_Trie> fail;
    std::list<std::pair<int, std::shared_ptr<AC_Trie>>> children;
    void add_fail();
};
