#pragma once

#include <memory>
#include <queue>
#include <list>

struct Node
{
    int value, children;
    double flow_to_parent;
    std::shared_ptr<Node> parent_node;
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

struct AC_Trie
{
    bool is_fail;
    int value;
    double flow;
    AC_Trie *fail;
    std::list<std::pair<int, AC_Trie>> children;
    void add_fail();
};
