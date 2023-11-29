#pragma once

#include <iostream>
#include <algorithm>
#include "../include/graph.hpp"
#include "../include/trie.hpp"

typedef std::vector<std::pair<int, double>>::iterator iterator;

struct Complete : public Graph
{
    std::vector<double> f_in, f_out;
    std::vector<std::pair<std::vector<iterator>, std::pair<int, double>>> path;

    std::vector<std::pair<double, std::vector<int>>> complete_repr;

    Complete(const std::string &graph);
    void insert(std::shared_ptr<AC_Trie> &root, std::vector<int> &str);
    void decompose_path();
    void compute_safe();
    void compress_path(double flow, std::deque<int> &route, std::shared_ptr<AC_Trie> &root);
    void print_maximal_safe_paths();
};
