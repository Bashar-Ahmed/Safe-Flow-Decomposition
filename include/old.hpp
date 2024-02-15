#pragma once

#include <iostream>
#include <algorithm>
#include "../include/graph.hpp"
#include "../include/trie.hpp"

typedef std::vector<std::pair<int, double>>::iterator iterator;
typedef std::tuple<int, int, double> cut;
template <typename T>
using path = std::pair<std::deque<int>, std::vector<T>>;
struct Old : public Graph
{
    bool concise, ac_trie;

    std::vector<double> f_in, f_out;
    std::vector<std::pair<std::vector<iterator>, std::pair<int, double>>> st_path;

    std::vector<std::pair<double, std::vector<int>>> raw_repr;
    std::vector<std::pair<double, std::deque<int>>> raw_repr_wo_trie;
    std::vector<path<cut>> concise_repr;

    Old(const std::string &graph, bool concise, bool ac_trie);
    template <typename T>
    void insert(std::shared_ptr<AC_Trie<T>> &root, std::vector<int> &str);
    void decompose_path();
    void compute_safe();
    template <typename T>
    void compress_path(double flow, std::deque<int> &route, std::shared_ptr<AC_Trie<T>> &root);
    void print_maximal_safe_paths();
};
