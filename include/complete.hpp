#pragma once

#include <iostream>
#include <algorithm>
#include "../include/graph.hpp"
#include "../include/trie.hpp"

typedef std::list<std::pair<int,double>>::iterator iterator;

struct Complete : public Graph {
    std::list<std::list<std::pair<int,double>>> reverse_adjacency_list;
    std::list<std::pair<std::list<iterator>,std::pair<int,double>>> path;
    std::list<std::pair<std::list<std::pair<int,double>>,std::pair<int,double>>> result;

    std::list<std::pair<double,std::list<int>>> complete_repr;

    Complete(const std::string& graph);
    void update(std::list<iterator>& list, double flow);
    void insert(AC_Trie root, std::list<int>* str);
    void decompose_path();
    void compute_safe();
    void compress_path();
    void print_maximal_safe_paths();
};
