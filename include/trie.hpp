#pragma once

#include <memory>
#include <queue>
#include "../include/node.hpp"

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

struct AC_Trie : public std::enable_shared_from_this<AC_Trie>
{
    bool is_fail;
    int value;
    double flow;
    std::weak_ptr<AC_Trie> fail;
    std::vector<std::pair<int, std::shared_ptr<AC_Trie>>> children;

    void add_fail()
    {
        std::shared_ptr<AC_Trie> shared_this = shared_from_this();
        this->fail = shared_this;
        std::queue<std::shared_ptr<AC_Trie>> q;
        q.push(shared_this);

        while (!q.empty())
        {
            auto source = q.front();
            q.pop();
            for (auto &&source_node : source->children)
            {
                auto target = source;
                source_node.second->fail.reset();
                while (target.get() != this)
                {
                    target = target->fail.lock();
                    for (auto &&target_node : target->children)
                    {
                        if (source_node.first == target_node.first)
                        {
                            source_node.second->fail = target_node.second;
                            target_node.second->is_fail = true;
                            target = shared_this;
                            break;
                        }
                    }
                }
                if (source_node.second->fail.lock().get() == nullptr)
                    source_node.second->fail = shared_this;
                q.push(source_node.second);
            }
        }
    };
};
