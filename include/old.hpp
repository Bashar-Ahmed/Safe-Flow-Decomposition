#pragma once

#include <iostream>
#include <algorithm>
#include "graph.hpp"
#include "trie.hpp"

typedef std::vector<std::pair<int, double>>::iterator iterator;
typedef std::tuple<int, int, double> cut;
template <typename T>
using path = std::pair<std::deque<int>, std::vector<T>>;

enum ALGO
{
    RAW,
    CONCISE
};

template <ALGO T>
struct Old : public Graph
{
    typedef std::conditional_t<T == RAW, double, std::vector<std::tuple<int, int, double>>> data;
    bool ac_trie;

    std::vector<double> f_in, f_out;
    std::vector<std::pair<std::vector<iterator>, std::pair<int, double>>> st_path;

    std::vector<std::pair<double, std::deque<int>>> raw_repr;
    std::vector<path<cut>> concise_repr;

    Old(const std::string &graph, bool ac_trie);
    void decompose_path();
    void insert(std::shared_ptr<AC_Trie<data>> &root, std::deque<int> &str);
    void compute_safe();
    void compress_path(data payload, std::deque<int> &route, std::shared_ptr<AC_Trie<data>> &root);
    void print_maximal_safe_paths();
};
