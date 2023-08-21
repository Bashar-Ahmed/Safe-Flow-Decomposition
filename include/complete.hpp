#pragma once

#include <iostream>
#include <algorithm>
#include "../include/graph.hpp"

typedef std::list<std::pair<int,double>>::iterator iterator;

struct Complete : public Graph {
    std::list<std::list<std::pair<int,double>>> reverse_adjacency_list;
    std::list<std::pair<std::list<iterator>,std::pair<int,double>>> path;
    std::list<std::pair<std::list<std::pair<int,double>>,std::pair<int,double>>> result;

    std::list<std::pair<double,std::list<int>>> complete_decomposition;

    Complete(const std::string& graph);
    void update(std::list<iterator>& list, double flow);
    void decompose_path();
    void compute_safe();
    void compress_path();
    void print_complete_decomposition();
};
