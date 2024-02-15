#pragma once

#include <memory>
#include <queue>

struct Raw_Node
{
    int value, children;
    double flow;
    std::shared_ptr<Raw_Node> parent;
    Raw_Node(int u) : value(u), children(0), flow(0.0) {}
};

struct Concise_Node
{
    int value, children;
    double flow;
    std::shared_ptr<Concise_Node> parent;
    std::weak_ptr<Concise_Node> v_max_in;
    Concise_Node(int u) : value(u), children(0), flow(0.0) {}
};

template <typename T>
    requires std::is_same_v<T, Raw_Node> || std::is_same_v<T, Concise_Node>
struct Path_Trie
{
    std::shared_ptr<T> head;
    Path_Trie(int u) : head(std::make_shared<T>(u)) {}

    std::shared_ptr<T> insert(int u, double flow, const std::shared_ptr<T> &parent)
    {
        std::shared_ptr<T> node;
        node = std::make_shared<T>(u);
        node->flow = flow;
        node->parent = parent;
        parent->children++;
        return node;
    }

    void merge(std::unique_ptr<Path_Trie<T>> &u, double flow)
    {

        u->head->parent = head;
        u->head->flow = flow;
        head->children++;
        return;
    }
};
template <typename T>
    requires std::is_same_v<T, double> || std::is_same_v<T, std::vector<std::tuple<int, int, double>>>
struct AC_Trie : public std::enable_shared_from_this<AC_Trie<T>>
{
    bool is_fail;
    int value;
    T payload;
    AC_Trie<T> *fail;
    std::vector<std::pair<int, std::shared_ptr<AC_Trie<T>>>> children;

    void add_fail()
    {
        this->fail = this;
        std::queue<std::shared_ptr<AC_Trie<T>>> q;
        q.push(this->shared_from_this());

        while (!q.empty())
        {
            auto source = q.front();
            q.pop();
            for (auto &&source_node : source->children)
            {
                AC_Trie<T> *target = source.get();
                source_node.second->fail = nullptr;
                while (target != this)
                {
                    target = target->fail;
                    for (auto &&target_node : target->children)
                    {
                        if (source_node.first == target_node.first)
                        {
                            source_node.second->fail = target_node.second.get();
                            target_node.second->is_fail = true;
                            target = this;
                            break;
                        }
                    }
                }
                if (source_node.second->fail == nullptr)
                    source_node.second->fail = this;
                q.push(source_node.second);
            }
        }
    };
};
