#pragma once

#include <graph.hpp>
#include <trie.hpp>

struct Raw : public Graph
{
    std::vector<double> f_in;
    std::vector<double> f_max_in;
    std::vector<int> v_max_in, topo_order;
    std::vector<std::unique_ptr<Path_Trie<Raw_Node>>> trie;
    std::vector<std::vector<std::pair<std::shared_ptr<Raw_Node>, double>>> leaves;

    std::vector<std::pair<double, std::vector<int>>> raw_repr;

    Raw(const std::string &graph);
    ~Raw();
    void compute_safe(int u);

    void print_safe_path(double flow, std::vector<int> &path);
    void print_safe_paths() override;

    void topo_dfs(int v, std::vector<bool> &visited);
};
