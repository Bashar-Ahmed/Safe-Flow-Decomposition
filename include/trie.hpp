#pragma once

#include <memory>
#include <unordered_map>
#include <map>
#include <queue>
#include <list>

enum class ALGO
{
    RAW,
    CONCISE
};

template <ALGO T>
struct Node
{
    int value, children;
    double flow_to_parent;
    std::shared_ptr<Node<T>> parent_node;

    struct empty
    {
    };
    using current_t = std::conditional_t<T == ALGO::CONCISE, std::map<int, std::shared_ptr<Node<T>>>, empty>;
    [[no_unique_address]] current_t child;

    Node(int u)
    {
        value = u;
        children = 0;
        flow_to_parent = 0.0;
        parent_node = nullptr;
    }

    Node(int u, double flow, const std::shared_ptr<Node> &parent)
    {
        value = u;
        children = 0;
        flow_to_parent = flow;
        parent_node = parent;
    }
};

template <ALGO T>
struct Path_Trie
{
    std::shared_ptr<Node<T>> head;

    Path_Trie(int u)
    {
        head = std::make_shared<Node<T>>(u);
    }

    std::shared_ptr<Node<T>> insert(int u, double flow, const std::shared_ptr<Node<T>> &parent)
    {
        std::shared_ptr<Node<T>> node = std::make_shared<Node<T>>(u, flow, parent);
        parent->children++;
        return node;
    }

    void merge(std::unique_ptr<Path_Trie<T>> &u, double flow)
    {
        u->head->parent_node = head;
        u->head->flow_to_parent = flow;
        head->children++;
        return;
    }
};

struct AC_Trie : public std::enable_shared_from_this<AC_Trie>
{
    bool is_fail;
    int value;
    double flow;
    std::weak_ptr<AC_Trie> fail;
    std::vector<std::pair<int, std::shared_ptr<AC_Trie>>> children;
    void add_fail();
};
