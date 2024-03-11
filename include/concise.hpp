#pragma once

#include <graph.hpp>
#include <trie.hpp>

typedef std::tuple<std::shared_ptr<Concise_Node>, int, double> Route;
typedef std::tuple<int, int, double> Cut;
template <typename T>
using Path = std::pair<std::vector<int>, std::vector<T>>;

template <bool H = false>
struct Concise : public Graph
{
    std::vector<double> f_in;
    std::vector<double> f_max_in;
    std::vector<int> v_max_in, topo_order;
    std::vector<std::unique_ptr<Path_Trie<Concise_Node>>> trie;
    std::vector<std::vector<int>> mark;
    std::vector<std::vector<Path<Route>>> partial_result;

    std::vector<Path<Cut>> concise_repr;

    Concise(const std::string &graph);
    ~Concise();
    void compute_safe(int u);

    void print_safe_paths() override;

    void topo_dfs(int v, std::vector<bool> &visited);
};