#pragma once

#include <iostream>
#include <algorithm>
#include "../include/graph.hpp"
#include "../include/trie.hpp"

typedef std::list<std::pair<int, double>>::iterator iterator;

struct Complete : public Graph
{
    std::vector<double> f_in, f_out;
    std::list<std::pair<std::list<iterator>, std::pair<int, double>>> path;

    std::list<std::pair<double, std::list<int>>> complete_repr;

    Complete(const std::string &graph);
    void update(std::list<iterator> &list, double flow);
    void insert(std::shared_ptr<AC_Trie> root, std::list<int> *str);
    void decompose_path();
    void compute_safe();
    void compress_path(double flow, std::list<int> &route, std::shared_ptr<AC_Trie> root);
    void print_maximal_safe_paths();
    void calculate_statistics();
};
